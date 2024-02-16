/**
 * @file config.h
 * @brief Configuration file for Wall-E robot.
 *
 * This file contains the configuration settings for Wall-E.
 * Modify the values in this file to setup or customize the behavior of WALL-E.
 *
 * @date 01/21/2024
 * @version 0.1
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP


/*---- Motor Index Mapping --------------------------------------------
*  The following constants are used to map the servo motors to the
*  corresponding index of the PWM module. The index is printed on the
*  silkscreen of the module.
*  -------------------------------------------------------------------*/
// Drive motors
#define MOTOR_LEFT_IDX  (0)
#define MOTOR_RIGHT_IDX (1)

// Arm servos
#define SERVO_SHOULDER_LEFT_IDX  (2)
#define SERVO_SHOULDER_RIGHT_IDX (3)

#define SERVO_ELBOW_LEFT_IDX  (4)
#define SERVO_ELBOW_RIGHT_IDX (5)

#define SERVO_WRIST_LEFT_IDX  (6)
#define SERVO_WRIST_RIGHT_IDX (7)

#define SERVO_HAND_LEFT_IDX   (8)
#define SERVO_HAND_RIGHT_IDX  (9)

// Head servos
#define SERVO_NECK_YAW_IDX    (15)
#define SERVO_NECK_PITCH_IDX  (14)
                        
#define SERVO_EYE_LEFT_IDX    (13)
#define SERVO_EYE_RIGHT_IDX   (12)

/*---- Track Motor Configs --------------------------------------------
*  The following constants setup parameters for the track motors.
*  -------------------------------------------------------------------*/
typedef struct TrackVelocityProfile{
    float speed_scaler;
    float acceleration;
} TrackVelocityProfile_t;

const TrackVelocityProfile_t TRACK_VELOCITY_PROFILES[] = {
    { .speed_scaler = 0.1, .acceleration = 1.00 }, // Slow
    { .speed_scaler = 0.3, .acceleration = 1.25 }, // Normal
    { .speed_scaler = 0.5, .acceleration = 1.50 }, // Fast
};
#define TRACK_VELOCITY_DEFAULT_PROFILE_IDX (1) // Index of the default velocity profile

/*---- Servo Head Motor Configs ---------------------------------------
*  The following constants setup phyiscal parameters of the servo 
*  motors. These should typically be configured and set once. 
*  -------------------------------------------------------------------*/
// Set the min and max us for the neck servos
#define SERVO_NECK_YAW_MAX_US (2500)
#define SERVO_NECK_YAW_MIN_US (700)
#define SERVO_NECK_YAW_NEUTRAL_US (1500)

#define SERVO_NECK_PITCH_MAX_US (1800)
#define SERVO_NECK_PITCH_MIN_US (1200)
#define SERVO_NECK_PITCH_NEUTRAL_US (1500)

// Set the min and max us for the arm servos
#define SERVO_SHOULDER_MAX_US (2000)
#define SERVO_SHOULDER_MIN_US (1000)
#define SERVO_SHOULDER_NEUTRAL_US (1500)

#define SERVO_ELBOW_MAX_US (2000)
#define SERVO_ELBOW_MIN_US (1000)
#define SERVO_ELBOW_NEUTRAL_US (1500)

#define SERVO_WRIST_MAX_US (2000)
#define SERVO_WRIST_MIN_US (1000)
#define SERVO_WRIST_NEUTRAL_US (1500)

#define SERVO_HAND_MAX_US (2000)
#define SERVO_HAND_MIN_US (1000)
#define SERVO_HAND_NEUTRAL_US (1500)

// Set the min and max us for the eye servos
#define SERVO_EYE_RIGHT_MAX_US (2000)
#define SERVO_EYE_RIGHT_MIN_US (1000)
#define SERVO_EYE_RIGHT_NEUTRAL_US (1500)

#define SERVO_EYE_LEFT_MAX_US (2000)
#define SERVO_EYE_LEFT_MIN_US (1000)
#define SERVO_EYE_LEFT_NEUTRAL_US (1500)

// Uncomment a line to reverse the direction of the track motor
// #define MOTOR_RIGHT_REVERSE_DIRECTION
// #define MOTOR_LEFT_REVERSE_DIRECTION

/*---- Audio Player Settings -------------------------------------------
*  Various settings of the Audio Player module.
*  -------------------------------------------------------------------*/
#define ENABLE_AUDIO // Comment this line to disable audio player
#define DEFAULT_AUDIO_VOLUME (24) // NOTE: Max volume is 30

// Index of audio tracks by their numbering on the SD card
#define TRACK_INDEX_STARTUP (1)
#define TRACK_INDEX_EVE_1   (2)
#define TRACK_INDEX_SCREAM  (3)
#define TRACK_INDEX_EVE_2   (4)
#define TRACK_INDEX_TADA    (5)
#define TRACK_INDEX_EVE_3   (6)
#define TRACK_INDEX_WALLE_1 (7)

// The track index to play on WALL-E startup
#define AUDIO_STARTUP_TRACK (TRACK_INDEX_STARTUP) // NOTE: Set to 0 if you don't want to play anything

// Tracks in this array will be selected randomly by the playRandomTrack() function
int audio_track_random_list[] = {
    TRACK_INDEX_EVE_1,
    TRACK_INDEX_EVE_2,
    TRACK_INDEX_EVE_3,
    TRACK_INDEX_WALLE_1,
    TRACK_INDEX_SCREAM,
    TRACK_INDEX_TADA,
};

// List of tracks to bind to controller buttons. The order is: 
// 0: up
// 1: right
// 2: down
// 3: left
int audio_track_selection_list[] = {
    TRACK_INDEX_EVE_1, // Up
    TRACK_INDEX_TADA, // Right
    TRACK_INDEX_SCREAM, // Down
    TRACK_INDEX_WALLE_1, // Left
};


/*---- Controller Settings ---------------------------------------------
*  Various settings for the controller and button inputs.
*  -------------------------------------------------------------------*/
#define WAIT_FOR_CONTROLLER_CONNECTION (false) // Set to false to disable waiting for controller connection
#define BUTTON_DEBOUNCE_TIME_MS (50) // Debounce time for button inputs in milliseconds

// Rates control how fast the given servo moves when using the controller. The values are in max travel distance per
// second. For a servo that travels between -90 and 90 degrees with 0 degrees being neutral, a value of 1 would move the
// servo from 0 to +-90 in one second. For analog sticks, this number maps to the maximum thumbstick positon and is
// scalled down based on the thumbstick position.
#define HEAD_YAW_RATE_PER_S   (2.0)
#define HEAD_PITCH_RATE_PER_S (1.0) 

#define EYE_MOVE_RATE_PER_S   (2.0)

#define SHOULDER_MOVE_RATE_PER_S (1.0)
#define ELBOW_MOVE_RATE_PER_S    (1.0)
#define WRIST_MOVE_RATE_PER_S    (1.0)
#define HAND_MOVE_RATE_PER_S     (1.0)

// The deadzone for the thumbsticks. This is the minimum value that the thumbstick must move before the controller
// registers input. Used to combat controller drift. Value is out of 512.
#define CONTROLLER_DEADZONE (25)

#endif /* CONFIG_HPP */
