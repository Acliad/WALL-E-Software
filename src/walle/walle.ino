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

#include "config.hpp"
#include "src/controller/navigation_controller.hpp"
#include "src/motion/drive_motor.hpp"
#include "src/display/display.hpp"
#include "solar_animations.hpp"
#include <Adafruit_PWMServoDriver.h>
#include <Bluepad32.h>
#include <DFMiniMp3.h>
#include <Wire.h>

/**************************************************************
 *                          Defines                           *
 **************************************************************/
#define PI_OVER_FOUR (PI / 4.0)

/**************************************************************
 *                         Constants                          *
 **************************************************************/
/*----------- PCA9685 PWM Module -------------------------*/
const unsigned int PCA9685_OSCILLATION_FREQ = 25000000; // TODO: Needs to be tuned?
const unsigned int SERVO_FREQ_HZ = 50; // NOTE: Analog servos run at ~50 Hz updates

/*----------- Servo/Motor --------------------------------*/
const unsigned int SERVO_NEUTRAL_US = 1500;
const unsigned int SERVO_MIN_US = 1000;
const unsigned int SERVO_MAX_US = 2000;

/*----------- Controllers --------------------------------*/
const unsigned int MAX_NUM_GAMEPADS = 2;

/************************************************************** 
 *                         Variables                          *
 **************************************************************/
/*----------- Display ------------------------------------*/
TFT_eSPI tft = TFT_eSPI();
SolarDisplay display = SolarDisplay(tft);

/*----------- PCA9685 PWM Module -------------------------*/
bool pca9685_connected = false;
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver();

/*----------- Track Motors -------------------------------*/
float left_motor_speed = 0.0f;
float right_motor_speed = 0.0f;
float motor_speed_factor = 0.5;
float motor_acceleration_per_ss = 1.5;
float motor_accelration_adjustment = 0.05; // Amount to adjust acceleration by when dpad buttons are pressed
// NOTE: The motor constructor assumes the PCA9685 has already been initialized
DriveMotor motor_r = DriveMotor(&pca9685, MOTOR_RIGHT_IDX);
DriveMotor motor_l = DriveMotor(&pca9685, MOTOR_LEFT_IDX);

/*----------- Head Servos --------------------------------*/
int servo_neck_yaw_us   = SERVO_NEUTRAL_US;
int servo_neck_pitch_us = SERVO_NEUTRAL_US;
int servo_eye_left_us   = SERVO_NEUTRAL_US;
int servo_eye_right_us  = SERVO_NEUTRAL_US;

/*----------- Arm Servos ---------------------------------*/
// ...

/*----------- Audio Player -------------------------------*/
class Mp3Notify;
// Handy typedef using serial and our notify class
typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3;
DfMp3_Status dfmp3_status;
unsigned int audio_current_track = 0;
unsigned int audio_num_tracks = 1;
DfMp3 dfmp3 = DfMp3(Serial2);


/*----------- Controllers --------------------------------*/
GamepadPtr           myGamepads[MAX_NUM_GAMEPADS];
NavigationController drive_controller;
NavigationController aux_controller;
// Keep an array of NavigationControllers to make connecting/disconnecting easier
NavigationController* controllers[MAX_NUM_GAMEPADS] = {&drive_controller, &aux_controller};

/**************************************************************
 *                    Function Prototypes                     *
 **************************************************************/
/*----------- Controllers --------------------------------*/
bool isGamepadConnected();
void mapThumbstick(int thumbstick_x, int thumbstick_y, float *left_motor_speed, float *right_motor_speed);

/*----------- General ------------------------------------*/
void updateAll();

void setup() {
    Serial.begin(115200);

    /*----------- Display --------------------------------*/
    display.begin();
    setup_animations();

    // Start the startup animation
    display.setAnimation(startup_animation);
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
    // Set the max speed to motor_speed_factor
    motor_r.set_speed_limit(motor_speed_factor);
    motor_l.set_speed_limit(motor_speed_factor);

    // Set the default acceleration
    motor_r.set_acceleration(motor_acceleration_per_ss);
    motor_l.set_acceleration(motor_acceleration_per_ss);

    /*----------- Controllers ----------------------------*/
    // Serial.println("Initializing Controller...");
    // BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
    // // Calling this can fix some issues with the controller not connecting
    // BP32.forgetBluetoothKeys();
    // Serial.println("Waiting for controller to connect...");
    // while (!isGamepadConnected()) {
    //     // Waiting...
    //     BP32.update();
    //     delay(100); // Not necessary, just prevents pinging isConnected() a ton
    // }

/***************************************************************
 *                     DFMini Audio Driver                      *
 ***************************************************************/
#ifdef ENABLE_AUDIO
    dfmp3.begin();
    dfmp3.reset(); // Could cause popping; can be removed after development.
    audio_num_tracks = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
    dfmp3.setVolume(DEFAULT_AUDIO_VOLUME);
    if (STARTUP_TRACK_INDEX) {
        dfmp3.playMp3FolderTrack(STARTUP_TRACK_INDEX);
    }
#endif
}

void loop() {

    updateAll();

    // Map right thumbstick (for now) to head motors
    // servo_neck_yaw_us = map(Ps3.data.analog.stick.lx, CONTROLLER_THUMBSTICK_MIN, CONTROLLER_THUMBSTICK_MAX, SERVO_MIN_US, SERVO_MAX_US);
    // servo_neck_pitch_us = map(Ps3.data.analog.stick.ly, CONTROLLER_THUMBSTICK_MIN, CONTROLLER_THUMBSTICK_MAX, SERVO_MIN_US, SERVO_MAX_US);

    // servo_neck_yaw_us = map(Ps3.data.analog.stick.lx, THUMBSTICK_MIN, CONTROLLER_THUMBSTICK_MAX, SERVO_MIN_US, SERVO_MAX_US);
    // servo_neck_pitch_us = map(Ps3.data.analog.stick.ly, CONTROLLER_THUMBSTICK_MIN, CONTROLLER_THUMBSTICK_MAX, SERVO_MIN_US, SERVO_MAX_US);

    // servo_eye_left_us = map(Ps3.event.analog_changed.button.l2, CONTROLLER_TRIGGER_MIN, CONTROLLER_TRIGGER_MAX, SERVO_MIN_US,
    // SERVO_MAX_US); servo_eye_right_us = map(Ps3.event.analog_changed.button.r2, CONTROLLER_TRIGGER_MIN, CONTROLLER_TRIGGER_MAX,
    // SERVO_MIN_US, SERVO_MAX_US);

    // Map thumbstick values to motor drive values
    mapThumbstick(drive_controller.thumbstickX(), 
                  -drive_controller.thumbstickY(), 
                  &left_motor_speed, 
                  &right_motor_speed
                  );

    if (pca9685_connected) {
        motor_r.set_speed(right_motor_speed);
        motor_l.set_speed(left_motor_speed);
    }

#ifdef ENABLE_AUDIO
    dfmp3.loop();
    dfmp3_status = dfmp3.getStatus();
    if (aux_controller.circleWasPressed() && (dfmp3_status.state != DfMp3_StatusState_Playing)) {
        Serial.print("Playing Track #");
        Serial.println(audio_current_track + 1);
        dfmp3.playMp3FolderTrack(audio_current_track + 1); // +1 for 1 indexing of filenames
        audio_current_track = (audio_current_track + 1) % audio_num_tracks;
    }
#endif

    // Adjust the motor acceleration curve if button pressed
    if (drive_controller.upWasPressed()) {
        motor_speed_factor += 0.05;
        motor_r.set_speed_limit(motor_speed_factor);
        motor_l.set_speed_limit(motor_speed_factor);
    }
    if (drive_controller.downWasPressed()) {
        motor_speed_factor -= 0.05;
        motor_r.set_speed_limit(motor_speed_factor);
        motor_l.set_speed_limit(motor_speed_factor);
    }
    // check if the right button was pressed
    if (drive_controller.rightWasPressed()) {
        motor_acceleration_per_ss += motor_accelration_adjustment;
        motor_r.set_acceleration(motor_acceleration_per_ss);
        motor_l.set_acceleration(motor_acceleration_per_ss);
    }
    // check if the left button was pressed
    if (drive_controller.leftWasPressed()) {
        motor_acceleration_per_ss -= motor_accelration_adjustment;
        motor_r.set_acceleration(motor_acceleration_per_ss);
        motor_l.set_acceleration(motor_acceleration_per_ss);
    }

    // Debug printing
    static long last_update_time = 0;
    if (millis() - last_update_time > 800) {
        last_update_time = millis();
        Serial.print("left_motor_speed = ");
        Serial.println(motor_l.get_current_speed());
        Serial.print("right_motor_speed = ");
        Serial.println(motor_r.get_current_speed());
        Serial.print("motor_acceleration_per_ss = ");
        Serial.println(motor_acceleration_per_ss);
        Serial.print("motor_speed_factor = ");
        Serial.println(motor_speed_factor);
        // Serial.print("calcul rmotor_us = ");
        // Serial.println(rmotor_us);
    }
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
        motor_r.update();
        motor_l.update();
    }

    /*----------- Controllers ----------------------------*/
    for (unsigned int i = 0; i < MAX_NUM_GAMEPADS; i++) {
        controllers[i]->update();
    }
    
}