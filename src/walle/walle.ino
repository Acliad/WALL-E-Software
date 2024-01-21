/**
 * @file walle.ino
 * @author Isaac Rex (@Acliad)
 * @brief Main program file for the Wall-E robot.
 * 
 *      TODO:
 *        - Document functions
 *        - Streamline the controller button parsing
 * @version 0.1
 * @date 2023-10-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <DFMiniMp3.h>
#include <Bluepad32.h>
#include "src/motion/drive_motor.hpp"

// #define MOTOR_RIGHT_REVERSE_DIRECTION
// #define MOTOR_LEFT_REVERSE_DIRECTION

// #define ENABLE_AUDIO

#define PCA9685_OSCILLATION_FREQ (25000000) // TODO: Needs to be tuned?
#define SERVO_FREQ_HZ (50) // Analog servos run at ~50 Hz updates

#define DEFAULT_AUDIO_VOLUME (24)
#define STARTUP_TRACK_INDEX (1) // Set to 0 if you don't want to play anything

// Servo/motor indices
#define MOTOR_LEFT_IDX       (0)
#define MOTOR_RIGHT_IDX      (1)
#define SERVO_NECK_YAW_IDX   (15)
#define SERVO_NECK_PTICH_IDX (14)
#define SERVO_EYE_LEFT_IDX   (13)
#define SERVO_EYE_RIGHT_IDX  (12)

#define THUMBSTICK_MAX (512)
#define THUMBSTICK_MIN (-511)
#define TRIGGER_MAX (255)
#define TRIGGER_MIN (0)

#define PI_OVER_FOUR (PI/4.0)

// Controller button masks
#define GAMEPAD_BUTTON_MASK_X   0x01
#define GAMEPAD_BUTTON_MASK_O   0x02
#define GAMEPAD_BUTTON_MASK_L1  0x10
#define GAMEPAD_BUTTON_MASK_L2  0x40
#define GAMEPAD_MISC_MASK_PS    0x01
#define GAMEPAD_DPAD_MASK_UP    0x01
#define GAMEPAD_DPAD_MASK_DOWN  0x02
#define GAMEPAD_DPAD_MASK_RIGHT 0x04
#define GAMEPAD_DPAD_MASK_LEFT  0x08

const unsigned int servo_neutral_us = 1500;
const unsigned int servo_min_us = 1000;
const unsigned int servo_max_us = 2000;

const unsigned int motor_range_us = servo_max_us - servo_min_us;
float left_motor_speed = 0.0f;
float right_motor_speed = 0.0f;

int servo_neck_yaw_us   = servo_neutral_us;
int servo_neck_pitch_us = servo_neutral_us;
int servo_eye_left_us   = servo_neutral_us;
int servo_eye_right_us  = servo_neutral_us;

unsigned int motor_ramp_up_time_ms   = 200;
unsigned int motor_ramp_down_time_ms = motor_ramp_up_time_ms;

float motor_speed_factor = 0.5;
float motor_acceleration_per_ss = 1.5;
float motor_accelration_adjustment = 0.05; // Amount to adjust acceleration by when dpad buttons are pressed

// Have to manually track button events as the controller library
// doesn't appear to poll frequently enough to properly capture button
bool circle_pressed_last = false;
bool circle_pressed = false;
bool d_up_pressed_last = false;
bool d_up_pressed = false;
bool d_down_pressed_last = false;
bool d_down_pressed = false;
bool d_right_pressed_last = false;
bool d_right_pressed = false;
bool d_left_pressed_last = false;
bool d_left_pressed = false;

bool pca9685_connected = false;

DfMp3_Status dfmp3_status;
unsigned int audio_current_track = 0;
unsigned int audio_num_tracks = 1;

GamepadPtr myGamepads[BP32_MAX_GAMEPADS];
GamepadPtr driveGamepad;
GamepadPtr auxGamepad;

// forward declare helper function to check if any gamepad is connected
bool isGamepadConnected();

// forward declare the notify class, just the name
class Mp3Notify; 
// define a handy type using serial and our notify class
typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3; 

DfMp3 dfmp3 = DfMp3(Serial2);
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver();
// NOTE: The motor constructor assumes the PCA9685 has already been initialized
DriveMotor motor_r = DriveMotor(&pca9685, MOTOR_RIGHT_IDX);
DriveMotor motor_l = DriveMotor(&pca9685, MOTOR_LEFT_IDX);

// Function prototypes
void map_thumbstick(int thumbstick_x, int thumbstick_y, float* left_motor_speed, float* right_motor_speed);

void setup() {
    Serial.begin(115200);

    /**************************************************************
    *                     PCA9685 Servo Driver                    *
    **************************************************************/
    Serial.println("Initializing PCA9685");
    pca9685_connected = pca9685.begin();
    if (!pca9685_connected) {
        Serial.println("************> Initialization failed...");
    } else {
        pca9685.setOscillatorFrequency(PCA9685_OSCILLATION_FREQ); 
        pca9685.setPWMFreq(SERVO_FREQ_HZ);  // Analog servos run at ~50 Hz updates
    }

    /**************************************************************
    *                         Drive Motors                        *
    **************************************************************/
    // Set the max speed to motor_speed_factor
    motor_r.set_speed_limit(motor_speed_factor);
    motor_l.set_speed_limit(motor_speed_factor);

    // Set the default acceleration
    motor_r.set_acceleration(motor_acceleration_per_ss);
    motor_l.set_acceleration(motor_acceleration_per_ss);
    

    /**************************************************************
    *                         Controller                          *
    **************************************************************/
    Serial.println("Initializing Controller...");
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
    // Calling this can fix some issues with the controller not connecting
    BP32.forgetBluetoothKeys();
    Serial.println("Waiting for controller to connect...");
    while(!isGamepadConnected()) {
        // Waiting...
        BP32.update();
        delay(100); // Not necessary, just prevents pinging isConnected() a ton
    }
    driveGamepad = myGamepads[0];
    Serial.println("Controller connected!");


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

    BP32.update();

    // Map right thumbstick (for now) to head motors
    // servo_neck_yaw_us = map(Ps3.data.analog.stick.lx, THUMBSTICK_MIN, THUMBSTICK_MAX, servo_min_us, servo_max_us);
    // servo_neck_pitch_us = map(Ps3.data.analog.stick.ly, THUMBSTICK_MIN, THUMBSTICK_MAX, servo_min_us, servo_max_us);

    // servo_neck_yaw_us = map(Ps3.data.analog.stick.lx, THUMBSTICK_MIN, THUMBSTICK_MAX, servo_min_us, servo_max_us);
    // servo_neck_pitch_us = map(Ps3.data.analog.stick.ly, THUMBSTICK_MIN, THUMBSTICK_MAX, servo_min_us, servo_max_us);

    // servo_eye_left_us = map(Ps3.event.analog_changed.button.l2, TRIGGER_MIN, TRIGGER_MAX, servo_min_us, servo_max_us);
    // servo_eye_right_us = map(Ps3.event.analog_changed.button.r2, TRIGGER_MIN, TRIGGER_MAX, servo_min_us, servo_max_us);

    // Map thumbstick values to motor drive values
    map_thumbstick(driveGamepad->axisX(), -driveGamepad->axisY(), &left_motor_speed, &right_motor_speed);

    if (pca9685_connected) {
        motor_r.set_speed(right_motor_speed);
        motor_l.set_speed(left_motor_speed);
        motor_r.update();
        motor_l.update();
    }

    #ifdef ENABLE_AUDIO
    dfmp3.loop(); 
    dfmp3_status = dfmp3.getStatus();
    circle_pressed = Ps3.data.analog.button.circle;
    if (!circle_pressed_last && circle_pressed && (dfmp3_status.state != DfMp3_StatusState_Playing)) { 
        Serial.print("Playing Track #");
        Serial.println(audio_current_track + 1);
        dfmp3.playMp3FolderTrack(audio_current_track + 1); // +1 for 1 indexing of filenames
        audio_current_track = (audio_current_track + 1) % audio_num_tracks;
    }
    circle_pressed_last = circle_pressed;
    #endif

    // update dpad buttons and adjust the motor acceleration curve if pressed
    unsigned int dpad_state = driveGamepad->dpad();
    d_up_pressed = dpad_state & GAMEPAD_DPAD_MASK_UP;
    d_down_pressed = dpad_state & GAMEPAD_DPAD_MASK_DOWN;
    d_right_pressed = dpad_state & GAMEPAD_DPAD_MASK_RIGHT;
    d_left_pressed = dpad_state & GAMEPAD_DPAD_MASK_LEFT;
    if (!d_up_pressed_last && d_up_pressed) {
        motor_speed_factor += 0.05;
        motor_r.set_speed_limit(motor_speed_factor);
        motor_l.set_speed_limit(motor_speed_factor);
        // motor_ramp_up_time_ms += 50;
        // motor_ramp_down_time_ms += 50;
        // motor_r.set_acceleration(motor_ramp_up_time_ms);
        // motor_l.set_acceleration(motor_ramp_up_time_ms);
        // motor_r.set_deceleration(motor_ramp_down_time_ms);
        // motor_l.set_deceleration(motor_ramp_down_time_ms);
    }
    if (!d_down_pressed_last && d_down_pressed) {
        motor_speed_factor -= 0.05;
        motor_r.set_speed_limit(motor_speed_factor);
        motor_l.set_speed_limit(motor_speed_factor);
        // motor_ramp_up_time_ms -= 50;
        // motor_ramp_down_time_ms -= 50;
        // motor_r.set_acceleration(motor_ramp_down_time_ms);
        // motor_l.set_acceleration(motor_ramp_down_time_ms);
        // motor_r.set_deceleration(motor_ramp_down_time_ms);
        // motor_l.set_deceleration(motor_ramp_down_time_ms);
    }
    // check if the right button was pressed
    if (!d_right_pressed_last && d_right_pressed) {
        motor_acceleration_per_ss += motor_accelration_adjustment;
        motor_r.set_acceleration(motor_acceleration_per_ss);
        motor_l.set_acceleration(motor_acceleration_per_ss);
    }
    // check if the left button was pressed
    if (!d_left_pressed_last && d_left_pressed) {
        motor_acceleration_per_ss -= motor_accelration_adjustment;
        motor_r.set_acceleration(motor_acceleration_per_ss);
        motor_l.set_acceleration(motor_acceleration_per_ss);
    }
    d_up_pressed_last = d_up_pressed;
    d_down_pressed_last = d_down_pressed;
    d_right_pressed_last = d_right_pressed;
    d_left_pressed_last = d_left_pressed;



    // Debug printing
    static long last_update_time = 0;
    if (millis() - last_update_time > 800) {
        last_update_time = millis();
        // Serial.print("Motor ramp time: ");
        // Serial.println(motor_ramp_up_time_ms);
        Serial.print("left_motor_speed = ");
        Serial.println(motor_l.get_current_speed());
        Serial.print("right_motor_speed = ");
        Serial.println(motor_r.get_current_speed());
        Serial.print("motor_acceleration_per_ss = ");
        Serial.println(motor_acceleration_per_ss);
        Serial.print("motor_speed_factor = ");
        Serial.println(motor_speed_factor);
        // Serial.print("event.button_down = ");
        // Serial.println(Ps3.event.button_down.circle);
        // Serial.print("data.analog.button.circle = ");
        // Serial.println(Ps3.data.analog.button.circle, DEC);
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
void map_thumbstick(int thumbstick_x, int thumbstick_y, float* left_motor_speed, float* right_motor_speed) {
    // Calculate the angle and magnitude of the thumbstick input
    double theta = atan2(thumbstick_y, thumbstick_x);
    double r = sqrt(thumbstick_x * thumbstick_x + thumbstick_y * thumbstick_y) / ((double) THUMBSTICK_MAX);

    // Ensure magnitude is within the valid range [0, 1]
    if (r > 1.0f) r = 1;

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


class Mp3Notify
{
public:
    static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action) {
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

    static void OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode)
    {
        // see DfMp3_Error for code meaning
        Serial.println();
        Serial.print("Com Error ");
        Serial.println(errorCode);
    }

    static void OnPlayFinished([[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track) {
        Serial.print("Play finished for #");
        Serial.println(track);  
    }

    static void OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
        PrintlnSourceAction(source, "online");
    }

    static void OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
        PrintlnSourceAction(source, "inserted");
    }

    static void OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
        PrintlnSourceAction(source, "removed");
    }
};

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedGamepad(GamepadPtr gp) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myGamepads[i] == nullptr) {
      Serial.printf("CALLBACK: Gamepad is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      GamepadProperties properties = gp->getProperties();
      Serial.printf("Gamepad model: %s, VID=0x%04x, PID=0x%04x\n",
                    gp->getModelName().c_str(), properties.vendor_id,
                    properties.product_id);
      myGamepads[i] = gp;
      foundEmptySlot = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println(
        "CALLBACK: Gamepad connected, but could not found empty slot");
  }
}

// This callback gets called any time a gamepad is disconnected.
void onDisconnectedGamepad(GamepadPtr gp) {
  bool foundGamepad = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myGamepads[i] == gp) {
      Serial.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
      myGamepads[i] = nullptr;
      foundGamepad = true;
      break;
    }
  }

  if (!foundGamepad) {
    Serial.println(
        "CALLBACK: Gamepad disconnected, but not found in myGamepads");
  }
}

bool isGamepadConnected() {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myGamepads[i] != nullptr) {
      return true;
    }
  }
  return false;
}