/**
 * @file walle.ino
 * @author Isaac Rex (@Acliad)
 * @brief Main program file for the Wall-E robot.
 *
 * @version 0.1
 * @date 2023-10-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "walle.hpp"

/**************************************************************
 *                         Constants                          *
 **************************************************************/
/*----------- PCA9685 PWM Module -------------------------*/
const unsigned int PCA9685_OSCILLATION_FREQ = 25000000; // TODO: Needs to be tuned?
const unsigned int SERVO_FREQ_HZ = 50; // NOTE: Analog servos run at ~50 Hz updates

/*----------- Controllers --------------------------------*/
const unsigned int MAX_NUM_GAMEPADS = 2;

/*----------- Buttons ------------------------------------*/
const unsigned int BUTTON_PIN_RECORD = 32;
const unsigned int BUTTON_PIN_PLAY   = 35;
const unsigned int BUTTON_PIN_STOP   = 34;
const unsigned int BUTTON_PIN_SUN    = 33;


/************************************************************** 
 *                         Variables                          *
 **************************************************************/
/*----------- Display ------------------------------------*/
TFT_eSPI tft = TFT_eSPI();
Display display = Display(tft);

/*----------- PCA9685 PWM Module -------------------------*/
bool pca9685_connected = false;
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver();

/*----------- Track Motors -------------------------------*/
float left_motor_speed = 0.0f;
float right_motor_speed = 0.0f;

int track_velocity_profile_idx = TRACK_VELOCITY_DEFAULT_PROFILE_IDX;
// NOTE: The motor constructor assumes the PCA9685 has already been initialized
DriveMotor motor_r = DriveMotor(&pca9685, MOTOR_RIGHT_IDX);
DriveMotor motor_l = DriveMotor(&pca9685, MOTOR_LEFT_IDX);

/*----------- Head Servos --------------------------------*/
float neck_yaw_position = 0.0f;
float neck_pitch_position = 0.0f;
float eye_left_position = 0.0f;
float eye_right_position = 0.0f;

// NOTE: Initializing with default parameters here, updated in initServos()
ServoMotor servo_neck_yaw(&pca9685, SERVO_NECK_YAW_IDX);
ServoMotor servo_neck_pitch(&pca9685, SERVO_NECK_PITCH_IDX);
ServoMotor servo_eye_left(&pca9685, SERVO_EYE_LEFT_IDX);
ServoMotor servo_eye_right(&pca9685, SERVO_EYE_RIGHT_IDX);

/*----------- Arm Servos ---------------------------------*/
float shoulder_left_position = 0.0f;
float shoulder_right_position = 0.0f;
float elbow_left_position = 0.0f;
float elbow_right_position = 0.0f;
float wrist_left_position = 0.0f;
float wrist_right_position = 0.0f;
float hand_left_position = 0.0f;
float hand_right_position = 0.0f;

ServoMotor servo_shoulder_left(&pca9685, SERVO_SHOULDER_LEFT_IDX);
ServoMotor servo_shoulder_right(&pca9685, SERVO_SHOULDER_RIGHT_IDX);
ServoMotor servo_elbow_left(&pca9685, SERVO_ELBOW_LEFT_IDX);
ServoMotor servo_elbow_right(&pca9685, SERVO_ELBOW_RIGHT_IDX);
ServoMotor servo_wrist_left(&pca9685, SERVO_WRIST_LEFT_IDX);
ServoMotor servo_wrist_right(&pca9685, SERVO_WRIST_RIGHT_IDX);
ServoMotor servo_hand_left(&pca9685, SERVO_HAND_LEFT_IDX);
ServoMotor servo_hand_right(&pca9685, SERVO_HAND_RIGHT_IDX);

ServoContext servo_context;


/*----------- Audio Player -------------------------------*/
class Mp3Notify;
// Handy typedef using serial and our notify class
typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3;

DfMp3_Status dfmp3_status;
unsigned int audio_current_track = 0;
unsigned int audio_num_tracks = 1;
DfMp3 dfmp3 = DfMp3(Serial2);

/*----------- Controllers/Buttons -------------------------*/
GamepadPtr           myGamepads[MAX_NUM_GAMEPADS];
NavigationController drive_controller;
NavigationController aux_controller;
// Keep an array of NavigationControllers to make connecting/disconnecting easier
NavigationController* controllers[MAX_NUM_GAMEPADS] = {&drive_controller, &aux_controller};

// Buttons
Button button_record = Button(BUTTON_PIN_RECORD, BUTTON_DEBOUNCE_TIME_MS);
Button button_play   = Button(BUTTON_PIN_PLAY,   BUTTON_DEBOUNCE_TIME_MS);
Button button_stop   = Button(BUTTON_PIN_STOP,   BUTTON_DEBOUNCE_TIME_MS);
Button button_sun    = Button(BUTTON_PIN_SUN,    BUTTON_DEBOUNCE_TIME_MS);

Button* buttons[] = {&button_record, &button_play, &button_stop, &button_sun};

/*----------- General ------------------------------------*/
enum class WallEState {
    NORMAL,
    RECORDING,
};
WallEState state = WallEState::NORMAL;
ServoAnimationRecorder* servo_recorder = nullptr;
Stats loop_stats = Stats(0.99);
ServoAnimation* head_animations[] = {&MotionAnimations::cock_left, &MotionAnimations::wiggle_eyes, &MotionAnimations::sad,
                                   &MotionAnimations::curious_track};
ServoPlayer& servo_player = ServoPlayer::getInstance();


/**************************************************************
 *                    Function Prototypes                     *
 **************************************************************/
/*----------- Controllers --------------------------------*/
bool isGamepadConnected();
void mapThumbstick(int thumbstick_x, int thumbstick_y, float *left_motor_speed, float *right_motor_speed);
void mapInputs(float dt);

/*----------- Audio Player -------------------------------*/
void playRandomTrack();

/*----------- General ------------------------------------*/
void updateAll();

void setup() {
    Serial.begin(115200);

    /*----------- Display --------------------------------*/
    display.begin();
    display.update(); // Update to draw the static text before animation starts
    DisplayAnimations::setup_animations();

    // Start the startup animation
    display.setAnimation(DisplayAnimations::startup);
    display.startAnimation();

    /*----------- PCA9685 PWM Module ----------------------*/
    Serial.println("Initializing PCA9685");
    pca9685_connected = pca9685.begin();
    if (!pca9685_connected) {
        Serial.println("************> Initialization failed...");
    } else {
        pca9685.setOscillatorFrequency(PCA9685_OSCILLATION_FREQ);
        pca9685.setPWMFreq(SERVO_FREQ_HZ);
    }

    /*----------- Drive Motors ---------------------------*/
    // Set the default max speed
    motor_r.set_speed_limit(TRACK_VELOCITY_PROFILES[track_velocity_profile_idx].speed_scaler);
    motor_l.set_speed_limit(TRACK_VELOCITY_PROFILES[track_velocity_profile_idx].speed_scaler);

    // Set the default acceleration
    motor_r.set_acceleration(TRACK_VELOCITY_PROFILES[track_velocity_profile_idx].acceleration);
    motor_l.set_acceleration(TRACK_VELOCITY_PROFILES[track_velocity_profile_idx].acceleration);

    /*----------- Servo Motors ---------------------------*/
    initServos();
    servo_context.servo_neck_yaw = &servo_neck_yaw;
    servo_context.servo_neck_pitch = &servo_neck_pitch;
    servo_context.servo_eye_left = &servo_eye_left;
    servo_context.servo_eye_right = &servo_eye_right;
    servo_context.servo_shoulder_left = &servo_shoulder_left;
    servo_context.servo_shoulder_right = &servo_shoulder_right;
    servo_context.servo_elbow_left = &servo_elbow_left;
    servo_context.servo_elbow_right = &servo_elbow_right;
    servo_context.servo_wrist_left = &servo_wrist_left;
    servo_context.servo_wrist_right = &servo_wrist_right;
    servo_context.servo_hand_left = &servo_hand_left;
    servo_context.servo_hand_right = &servo_hand_right;

    MotionAnimations::setup_animations(servo_context);

    /*----------- Controllers ----------------------------*/
    Serial.println("Initializing Controller...");
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
    // Calling this can fix some issues with the controller not connecting
    BP32.forgetBluetoothKeys();
    Serial.println("Waiting for controller to connect...");
    while (!isGamepadConnected() && WAIT_FOR_CONTROLLER_CONNECTION) {
        // Waiting...
        BP32.update();
        delay(100); // Not necessary, just prevents pinging isConnected() a ton
    }
    drive_controller.setDeadzone(CONTROLLER_DEADZONE);
    aux_controller.setDeadzone(CONTROLLER_DEADZONE);

/***************************************************************
 *                     DFMini Audio Driver                      *
 ***************************************************************/
#ifdef ENABLE_AUDIO
    dfmp3.begin();
    dfmp3.reset(); // Could cause popping; can be removed after development.
    audio_num_tracks = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
    dfmp3.setVolume(DEFAULT_AUDIO_VOLUME);
    if (TRACK_INDEX_STARTUP) {
        dfmp3.playMp3FolderTrack(TRACK_INDEX_STARTUP);
    }
#endif
}

void loop() {
    static unsigned long loop_start_time = millis();
    float dt = (float) (millis() - loop_start_time) / 1000.0;
    loop_start_time = millis();

    updateAll();
    mapInputs(dt);

    // Update the loop stats
    static int test = 0;
    if (test > 1)
        loop_stats.addNumber(millis() - loop_start_time);
    else
        test++;
    // Debug printing
    static long last_update_time = 0;
    if (millis() - last_update_time > 800) {
        last_update_time = millis();

        // Serial.println("--------------------------------------------------");
        // Serial.print("Free heap bytes: ");
        // Serial.println(ESP.getFreeHeap());
        // Serial.println("--------------------------------------------------");

    }
}

void initServos() {
    // Set ramp mode to SINUSOIDAL_INOUT for all motors
    servo_neck_yaw.set_ramp_mode(SINUSOIDAL_INOUT);
    servo_neck_pitch.set_ramp_mode(SINUSOIDAL_INOUT);
    servo_eye_left.set_ramp_mode(SINUSOIDAL_INOUT);
    servo_eye_right.set_ramp_mode(SINUSOIDAL_INOUT);
    servo_shoulder_left.set_ramp_mode(SINUSOIDAL_INOUT);
    servo_shoulder_right.set_ramp_mode(SINUSOIDAL_INOUT);
    servo_elbow_left.set_ramp_mode(SINUSOIDAL_INOUT);
    servo_elbow_right.set_ramp_mode(SINUSOIDAL_INOUT);
    servo_wrist_left.set_ramp_mode(SINUSOIDAL_INOUT);
    servo_wrist_right.set_ramp_mode(SINUSOIDAL_INOUT);
    servo_hand_left.set_ramp_mode(SINUSOIDAL_INOUT);
    servo_hand_right.set_ramp_mode(SINUSOIDAL_INOUT);


    /*************************************
     * Setup the eye servos
     *************************************/
    // Set the min and max us for the eye servos
    servo_eye_right.set_max_us(SERVO_EYE_RIGHT_MAX_US);
    servo_eye_right.set_min_us(SERVO_EYE_RIGHT_MIN_US);
    servo_eye_right.set_neutral_us(SERVO_EYE_RIGHT_NEUTRAL_US);
    // left eye
    servo_eye_left.set_max_us(SERVO_EYE_LEFT_MAX_US);
    servo_eye_left.set_min_us(SERVO_EYE_LEFT_MIN_US);
    servo_eye_left.set_neutral_us(SERVO_EYE_LEFT_NEUTRAL_US);

    /*************************************
     * Setup the neck servos
     *************************************/
    servo_neck_pitch.set_max_us(SERVO_NECK_PITCH_MAX_US);
    servo_neck_pitch.set_min_us(SERVO_NECK_PITCH_MIN_US);
    servo_neck_pitch.set_neutral_us(SERVO_NECK_PITCH_NEUTRAL_US);

    servo_neck_yaw.set_max_us(SERVO_NECK_YAW_MAX_US);
    servo_neck_yaw.set_min_us(SERVO_NECK_YAW_MIN_US);
    servo_neck_yaw.set_neutral_us(SERVO_NECK_YAW_NEUTRAL_US);

    /*************************************
     * Setup the arm servos
     *************************************/
    servo_shoulder_left.set_max_us(SERVO_SHOULDER_MAX_US);
    servo_shoulder_left.set_min_us(SERVO_SHOULDER_MIN_US);
    servo_shoulder_left.set_neutral_us(SERVO_SHOULDER_NEUTRAL_US);

    servo_shoulder_right.set_max_us(SERVO_SHOULDER_MAX_US);
    servo_shoulder_right.set_min_us(SERVO_SHOULDER_MIN_US);
    servo_shoulder_right.set_neutral_us(SERVO_SHOULDER_NEUTRAL_US);

    servo_elbow_left.set_max_us(SERVO_ELBOW_MAX_US);
    servo_elbow_left.set_min_us(SERVO_ELBOW_MIN_US);
    servo_elbow_left.set_neutral_us(SERVO_ELBOW_NEUTRAL_US);

    servo_elbow_right.set_max_us(SERVO_ELBOW_MAX_US);
    servo_elbow_right.set_min_us(SERVO_ELBOW_MIN_US);
    servo_elbow_right.set_neutral_us(SERVO_ELBOW_NEUTRAL_US);

    servo_wrist_left.set_max_us(SERVO_WRIST_MAX_US);
    servo_wrist_left.set_min_us(SERVO_WRIST_MIN_US);
    servo_wrist_left.set_neutral_us(SERVO_WRIST_NEUTRAL_US);

    servo_wrist_right.set_max_us(SERVO_WRIST_MAX_US);
    servo_wrist_right.set_min_us(SERVO_WRIST_MIN_US);
    servo_wrist_right.set_neutral_us(SERVO_WRIST_NEUTRAL_US);

    servo_hand_left.set_max_us(SERVO_HAND_MAX_US);
    servo_hand_left.set_min_us(SERVO_HAND_MIN_US);
    servo_hand_left.set_neutral_us(SERVO_HAND_NEUTRAL_US);

    servo_hand_right.set_max_us(SERVO_HAND_MAX_US);
    servo_hand_right.set_min_us(SERVO_HAND_MIN_US);
    servo_hand_right.set_neutral_us(SERVO_HAND_NEUTRAL_US);

    // Set all motors to neutral
    servo_neck_yaw.set_angle(0.0f, 0);
    servo_neck_pitch.set_angle(0.0f, 0);

    servo_eye_left.set_scalar(0.0f, 0);
    servo_eye_right.set_scalar(0.0f, 0);

    servo_shoulder_left.set_scalar(0.0f, 0);
    servo_shoulder_right.set_scalar(0.0f, 0);

    servo_elbow_left.set_scalar(0.0f, 0);
    servo_elbow_right.set_scalar(0.0f, 0);

    servo_wrist_left.set_scalar(0.0f, 0);
    servo_wrist_right.set_scalar(0.0f, 0);

    servo_hand_left.set_scalar(0.0f, 0);
    servo_hand_right.set_scalar(0.0f, 0);
}

/**
 * @brief Maps thumbstick input to motor thrust values.
 *
 * This function takes thumbstick input coordinates and maps them to motor thrust values for a robotic system. It
 * calculates the angle and magnitude of the input and maps it to motor thrusts in four quadrants. Additionally, it
 * allows for reversing the direction of individual motors if needed.
 *
 * NOTE: The navigations controllers have rather large deadzones around x = -180° and 0°. Same for y = 90° and -90°. I
 * may explore compensation later.
 *
 * @param thumbstick_x The x-coordinate of the thumbstick input.
 * @param thumbstick_y The y-coordinate of the thumbstick input.
 * @param[out] lmotor_us Pointer to store the left motor pulse width value (microseconds).
 * @param[out] rmotor_us Pointer to store the right motor pulse width value (microseconds).
 */
void mapThumbstick(int thumbstick_x, int thumbstick_y, float *left_motor_speed, float *right_motor_speed) {
    // Calculate the angle and magnitude of the thumbstick input
    double theta = atan2(thumbstick_y, thumbstick_x);
    double r = sqrt(thumbstick_x * thumbstick_x + thumbstick_y * thumbstick_y) / ((double)CONTROLLER_THUMBSTICK_MAX);

    // Ensure magnitude is within the valid range [0, 1]
    if (r > 1.0)
        r = 1.0;

    double rmotor_thrust;
    double lmotor_thrust;

    // Map thumbstick input to motor thrusts in four quadrants
    if (theta >= 0.0 && theta <= HALF_PI) {
        // Quadrant 1
        rmotor_thrust = r * (theta - PI_OVER_FOUR) / PI_OVER_FOUR;
        lmotor_thrust = r;
    }
    if (theta > HALF_PI && theta <= PI) {
        // Quadrant 2
        rmotor_thrust = r;
        lmotor_thrust = r * (PI_OVER_FOUR - (theta - HALF_PI)) / PI_OVER_FOUR;
    }
    if (theta >= -PI && theta <= -HALF_PI) {
        // Quadrant 3
        rmotor_thrust = r * (-(theta + HALF_PI) - PI_OVER_FOUR) / PI_OVER_FOUR;
        lmotor_thrust = -r;
    }
    if (theta > -HALF_PI && theta < 0.0) {
        // Quadrant 4
        rmotor_thrust = -r;
        lmotor_thrust = r * (theta + HALF_PI - PI_OVER_FOUR) / PI_OVER_FOUR;
    }

// Reverse motor directions if specified
#ifdef MOTOR_RIGHT_REVERSE_DIRECTION
    rmotor_thrust *= -1;
#endif
#ifdef MOTOR_LEFT_REVERSE_DIRECTION
    lmotor_thrust *= -1;
#endif

    // Calculate motor pulse width values and store them
    *right_motor_speed = rmotor_thrust;
    *left_motor_speed = lmotor_thrust;
}

/**
 * @brief The Mp3Notify class provides static methods for handling MP3 player events.
 */
class Mp3Notify {
  public:
    /**
     * @brief Prints the source and action of the MP3 player event.
     * 
     * @param source The play source(s) of the MP3 player event.
     * @param action The action performed by the MP3 player event.
     */
    static void PrintlnSourceAction(DfMp3_PlaySources source, const char *action) {
        if (source & DfMp3_PlaySources_Sd) {
            Serial.print("SD Card, ");
        }
        if (source & DfMp3_PlaySources_Usb) {
            Serial.print("USB Disk, ");
        }
        if (source & DfMp3_PlaySources_Flash) {
            Serial.print("Flash, ");
        }
        Serial.println(action);
    }

    /**
     * @brief Handles the error event of the MP3 player.
     * 
     * @param mp3 The MP3 player instance.
     * @param errorCode The error code indicating the type of error.
     */
    static void OnError([[maybe_unused]] DfMp3 &mp3, uint16_t errorCode) {
        // see DfMp3_Error for code meaning
        Serial.println();
        Serial.print("Com Error ");
        Serial.println(errorCode);
    }

    /**
     * @brief Handles the play finished event of the MP3 player.
     * 
     * @param mp3 The MP3 player instance.
     * @param source The play source(s) of the MP3 player event.
     * @param track The track number that finished playing.
     */
    static void OnPlayFinished([[maybe_unused]] DfMp3 &mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track) {
        Serial.print("Play finished for #");
        Serial.println(track);
    }

    /**
     * @brief Handles the play source online event of the MP3 player.
     * 
     * @param mp3 The MP3 player instance.
     * @param source The play source(s) of the MP3 player event.
     */
    static void OnPlaySourceOnline([[maybe_unused]] DfMp3 &mp3, DfMp3_PlaySources source) {
        PrintlnSourceAction(source, "online");
    }

    /**
     * @brief Handles the play source inserted event of the MP3 player.
     * 
     * @param mp3 The MP3 player instance.
     * @param source The play source(s) of the MP3 player event.
     */
    static void OnPlaySourceInserted([[maybe_unused]] DfMp3 &mp3, DfMp3_PlaySources source) {
        PrintlnSourceAction(source, "inserted");
    }

    /**
     * @brief Handles the play source removed event of the MP3 player.
     * 
     * @param mp3 The MP3 player instance.
     * @param source The play source(s) of the MP3 player event.
     */
    static void OnPlaySourceRemoved([[maybe_unused]] DfMp3 &mp3, DfMp3_PlaySources source) {
        PrintlnSourceAction(source, "removed");
    }
};

/**
 * Callback function called when a gamepad is connected.
 * It assigns the gamepad to an empty slot in the controllers array.
 * If no empty slot is found, it prints an error message.
 * 
 * @param gp The connected gamepad.
 */
void onConnectedGamepad(GamepadPtr gp) {
    bool foundEmptySlot = false;
    for (int i = 0; i < MAX_NUM_GAMEPADS; i++) {
        if (controllers[i]->getGamepad() == nullptr) {
            controllers[i]->setGamepad(gp);
            foundEmptySlot = true;
            // NOTE: There seemst to be a bug related to scan_evt that will cause the controllers to sometimes
            // discconect and print a message in the console like: "FEX 7 0" or "FEX 7 1". Refer to #43
            // https://github.com/ricardoquesada/bluepad32/issues/43 for more information. That issue reported that
            // disabling new connections would fix the issue, so here's the workaround. 
            if (i == MAX_NUM_GAMEPADS-1) {
                BP32.enableNewBluetoothConnections(false);
            }
            Serial.printf("CALLBACK: Gamepad is connected, index=%d\n", i);
            break;
        }
    }
    if (!foundEmptySlot) {
        Serial.println("CALLBACK: Gamepad connected, but could not found empty slot");
    }
}

/**
 * Callback function called when a gamepad is disconnected.
 * It searches for the disconnected gamepad in the list of controllers and updates the corresponding entry.
 * If the gamepad is not found in the list, it prints a message indicating that the gamepad was disconnected but not 
 * found.
 * 
 * @param gp The disconnected gamepad.
 */
void onDisconnectedGamepad(GamepadPtr gp) {
    // If the controller is the main controller, stop WALL-E
    if (gp == drive_controller.getGamepad()) {
        if (pca9685_connected) {
            motor_r.set_speed(0);
            motor_l.set_speed(0);
        }
    }

    // Find the gamepad in the list of controllers and update the entry
    bool foundGamepad = false;
    for (int i = 0; i < MAX_NUM_GAMEPADS; i++) {
        if (controllers[i]->getGamepad() == gp) {
            Serial.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
            controllers[i]->setGamepad(nullptr);
            foundGamepad = true;
            break;
        }
    }

    if (!foundGamepad) {
        Serial.println("CALLBACK: Gamepad disconnected, but not found in myGamepads");
    }

    // NOTE: See note in onConnectedGamepad() for more information
    BP32.enableNewBluetoothConnections(true);
}

/**
 * Checks if any gamepad is connected.
 * 
 * @return true if at least one gamepad is connected, false otherwise.
 */
bool isGamepadConnected() {
    for (int i = 0; i < MAX_NUM_GAMEPADS; i++) {
        if (controllers[i]->getGamepad() != nullptr) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Updates all components of the Wall-E robot with update functions.
 * 
 * This function updates the display, motors/servos, and controllers of the Wall-E robot.
 * It is called periodically to ensure that all components are functioning properly.
 */
void updateAll() {
    /*----------- Display --------------------------------*/
    display.update();

    /*----------- Motors/Servors -------------------------*/
    if (pca9685_connected) {
        // Update motors
        motor_r.set_speed(right_motor_speed);
        motor_l.set_speed(left_motor_speed);
        motor_r.update();
        motor_l.update();

        // Update servos unless animating
        if (servo_player.isPlaying()) {
            servo_player.update();

            // Update the tracked positions
            neck_yaw_position = servo_neck_yaw.get_scalar();
            neck_pitch_position = servo_neck_pitch.get_scalar();

            eye_left_position = servo_eye_left.get_scalar();
            eye_right_position = servo_eye_right.get_scalar();

            shoulder_left_position = servo_shoulder_left.get_scalar();
            shoulder_right_position = servo_shoulder_right.get_scalar();

            elbow_left_position = servo_elbow_left.get_scalar();
            elbow_right_position = servo_elbow_right.get_scalar();

            wrist_left_position = servo_wrist_left.get_scalar();
            wrist_right_position = servo_wrist_right.get_scalar();

            hand_left_position = servo_hand_left.get_scalar();
            hand_right_position = servo_hand_right.get_scalar();
        } else {
            // Set servo positions
            servo_neck_yaw.set_scalar(neck_yaw_position, 0);
            servo_neck_pitch.set_scalar(neck_pitch_position, 0);

            servo_eye_left.set_scalar(eye_left_position, 0);
            servo_eye_right.set_scalar(eye_right_position, 0);

            servo_shoulder_left.set_scalar(shoulder_left_position, 0);
            servo_shoulder_right.set_scalar(shoulder_right_position, 0);
            
            servo_elbow_left.set_scalar(elbow_left_position, 0);
            servo_elbow_right.set_scalar(elbow_right_position, 0);

            servo_wrist_left.set_scalar(wrist_left_position, 0);
            servo_wrist_right.set_scalar(wrist_right_position, 0);

            servo_hand_left.set_scalar(hand_left_position, 0);
            servo_hand_right.set_scalar(hand_right_position, 0);

            servo_neck_yaw.update();
            servo_neck_pitch.update();
            servo_eye_left.update();
            servo_eye_right.update();
            servo_shoulder_left.update();
            servo_shoulder_right.update();
            servo_elbow_left.update();
            servo_elbow_right.update();
            servo_wrist_left.update();
            servo_wrist_right.update();
            servo_hand_left.update();
            servo_hand_right.update();
        }
    }

#ifdef ENABLE_AUDIO
    /*----------- Audio Player ---------------------------*/
    dfmp3.loop();
#endif

    /*----------- Controllers ----------------------------*/
    for (unsigned int i = 0; i < MAX_NUM_GAMEPADS; i++) {
        controllers[i]->update();
    }
    
    /*----------- Buttons --------------------------------*/
    for (unsigned int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
        buttons[i]->update();
    }
}

/**
 * @brief Plays a random track from the audio_track_random_list array. This list is defined in config.hpp.
 * 
 */
void playRandomTrack() {
    int random_index = random(sizeof(audio_track_random_list) / sizeof(audio_track_random_list[0]));
    int track_index = audio_track_random_list[random_index];
    dfmp3.playMp3FolderTrack(track_index);
}

void mapInputs(float dt) {
    /*----------- Motor Speed ----------------------------*/
    mapThumbstick(drive_controller.thumbstickX(), -drive_controller.thumbstickY(), &left_motor_speed,
                  &right_motor_speed);
    if (state == WallEState::NORMAL) {
        if (drive_controller.xWasPressed()) {
            track_velocity_profile_idx = min((track_velocity_profile_idx + 1), ARRAY_SIZE(TRACK_VELOCITY_PROFILES));
            float motor_speed_factor = TRACK_VELOCITY_PROFILES[track_velocity_profile_idx].speed_scaler;
            float motor_acceleration = TRACK_VELOCITY_PROFILES[track_velocity_profile_idx].acceleration;

            motor_r.set_speed_limit(motor_speed_factor);
            motor_l.set_speed_limit(motor_speed_factor);
            motor_r.set_acceleration(motor_acceleration);
            motor_l.set_acceleration(motor_acceleration);
        }

        if (drive_controller.circleWasPressed()) {
            track_velocity_profile_idx = max((track_velocity_profile_idx - 1), 0);
            float motor_speed_factor = TRACK_VELOCITY_PROFILES[track_velocity_profile_idx].speed_scaler;
            float motor_acceleration = TRACK_VELOCITY_PROFILES[track_velocity_profile_idx].acceleration;

            motor_r.set_speed_limit(motor_speed_factor);
            motor_l.set_speed_limit(motor_speed_factor);
            motor_r.set_acceleration(motor_acceleration);
            motor_l.set_acceleration(motor_acceleration);
        }
    }
    /*----------- Head Movement --------------------------*/
    if (!(aux_controller.l2IsPressed() || drive_controller.l2IsPressed() || drive_controller.l1IsPressed())) {
        // If not relavent modifier is pressed...
        neck_pitch_position =
            constrain(neck_pitch_position + HEAD_YAW_RATE_PER_S * -aux_controller.thumbstickYNorm() * dt, -1.0f, 1.0f);
        neck_yaw_position =
            constrain(neck_yaw_position + HEAD_PITCH_RATE_PER_S * aux_controller.thumbstickXNorm() * dt, -1.0f, 1.0f);
    }

    if (aux_controller.l2IsPressed()) {
        eye_left_position =
            constrain(eye_left_position + EYE_MOVE_RATE_PER_S * -drive_controller.thumbstickYNorm() * dt, -1.0f, 1.0f);
        eye_right_position =
            constrain(eye_right_position + EYE_MOVE_RATE_PER_S * aux_controller.thumbstickYNorm() * dt, -1.0f, 1.0f);
    }
    /*----------- Arm Movement ---------------------------*/
    // TODO: Map this out
    if (drive_controller.l2IsPressed()) {
        // ----------- Shoulders -----------
        shoulder_left_position = constrain(
            shoulder_left_position + SHOULDER_MOVE_RATE_PER_S * -drive_controller.thumbstickYNorm() * dt, -1.0f, 1.0f);
        shoulder_right_position = constrain(
            shoulder_right_position + SHOULDER_MOVE_RATE_PER_S * -aux_controller.thumbstickYNorm() * dt, -1.0f, 1.0f);

        // ------------ Elbows -------------
        elbow_left_position = constrain(
            elbow_left_position + ELBOW_MOVE_RATE_PER_S * drive_controller.thumbstickXNorm() * dt, -1.0f, 1.0f);
        elbow_right_position = constrain(
            elbow_right_position + ELBOW_MOVE_RATE_PER_S * aux_controller.thumbstickXNorm() * dt, -1.0f, 1.0f);

    } else if (drive_controller.l1IsPressed()) {
        // ------------- Wrists ------------
        wrist_left_position = constrain(
            wrist_left_position + WRIST_MOVE_RATE_PER_S * drive_controller.thumbstickXNorm() * dt, -1.0f, 1.0f);
        wrist_right_position = constrain(
            wrist_right_position + WRIST_MOVE_RATE_PER_S * aux_controller.thumbstickXNorm() * dt, -1.0f, 1.0f);

        // ------------- Hands -------------
        hand_left_position = constrain(
            hand_left_position + HAND_MOVE_RATE_PER_S * -drive_controller.thumbstickYNorm() * dt, -1.0f, 1.0f);
        hand_right_position = constrain(
            hand_right_position + HAND_MOVE_RATE_PER_S * -aux_controller.thumbstickYNorm() * dt, -1.0f, 1.0f);
    }

    /*----------- Animations -----------------------------*/
    if (state == WallEState::NORMAL) {
        if (drive_controller.upWasPressed()) {
            servo_player.play(head_animations[0]);
        } else if (drive_controller.rightWasPressed()) {
            servo_player.play(head_animations[1]);
        } else if (drive_controller.downWasPressed()) {
            servo_player.play(head_animations[2]);
        } else if (drive_controller.leftWasPressed()) {
            servo_player.play(head_animations[3]);
        }
        if (drive_controller.thumbstickWasPressed()) {
            servo_player.stop();
        }
    }

    /*----------- Sounds ---------------------------------*/
    if (aux_controller.upWasPressed()) {
        dfmp3.playMp3FolderTrack(audio_track_selection_list[0]);
    } else if (aux_controller.rightWasPressed()) {
        dfmp3.playMp3FolderTrack(audio_track_selection_list[1]);
    } else if (aux_controller.downWasPressed()) {
        dfmp3.playMp3FolderTrack(audio_track_selection_list[2]);
    } else if (aux_controller.leftWasPressed()) {
        dfmp3.playMp3FolderTrack(audio_track_selection_list[3]);
    }

    /*----------- General ---------------------------------*/
    if (aux_controller.psWasPressed()) {
        // Swap this controller to the main controller
        GamepadPtr temp = drive_controller.getGamepad();
        drive_controller.setGamepad(aux_controller.getGamepad());
        aux_controller.setGamepad(temp);
    }

    /*----------- Buttons --------------------------------*/
    if (button_record.wasPressed() && state == WallEState::NORMAL) {
        state = WallEState::RECORDING;
        servo_recorder = new ServoAnimationRecorder(display, servo_context);
    }


    /*----------- Recording Mode Inputs ------------------*/
    if (state == WallEState::RECORDING) {
        ServoAnimationRecorder::States recorder_state = servo_recorder->getState();
        if (button_stop.wasPressed()) {
            recorder_state = servo_recorder->inputEvent(ServoAnimationRecorder::Inputs::CANCEL);
        } else if (button_play.wasPressed()) {
            recorder_state = servo_recorder->inputEvent(ServoAnimationRecorder::Inputs::DONE);
        } else if (drive_controller.upWasPressed()) {
            recorder_state = servo_recorder->inputEvent(ServoAnimationRecorder::Inputs::UP);
        } else if (drive_controller.rightWasPressed()) {
            recorder_state = servo_recorder->inputEvent(ServoAnimationRecorder::Inputs::RIGHT);
        } else if (drive_controller.downWasPressed()) {
            recorder_state = servo_recorder->inputEvent(ServoAnimationRecorder::Inputs::DOWN);
        } else if (drive_controller.leftWasPressed()) {
            recorder_state = servo_recorder->inputEvent(ServoAnimationRecorder::Inputs::LEFT);
        } else if (drive_controller.xWasPressed()) {
            recorder_state = servo_recorder->inputEvent(ServoAnimationRecorder::Inputs::PREV);
        } else if (drive_controller.circleWasPressed()) {
            recorder_state = servo_recorder->inputEvent(ServoAnimationRecorder::Inputs::NEXT);
        } else if (aux_controller.xWasPressed()) {
            recorder_state = servo_recorder->inputEvent(ServoAnimationRecorder::Inputs::CANCEL);
        } else if (aux_controller.circleWasPressed()) {
            recorder_state = servo_recorder->inputEvent(ServoAnimationRecorder::Inputs::DONE);
        // DEBUG >>>>>>>>>>>>>>>>>>>
        } else if (button_sun.wasPressed()) { // TODO: Remove me when done debugging
            recorder_state = servo_recorder->inputEvent(ServoAnimationRecorder::Inputs::UP);
        }
        // DEBUG <<<<<<<<<<<<<<<<<<<<

        if (recorder_state == ServoAnimationRecorder::States::DONE) {
            std::unique_ptr<ServoAnimation> animation = servo_recorder->takeAnimation();
            head_animations[0] = animation.release();
            state = WallEState::NORMAL;
            delete servo_recorder;
            servo_recorder = nullptr;
        }
    }
}