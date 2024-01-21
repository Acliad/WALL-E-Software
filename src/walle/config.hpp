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

/*---- Servo Motor Index Mapping ---------------------------------------
*  The following constants are used to map the servo motors to the
*  corresponding index of the PWM module. The index is printed on the
*  silkscreen of the module.
*  -------------------------------------------------------------------*/
#define MOTOR_LEFT_IDX       (0)
#define MOTOR_RIGHT_IDX      (1)
#define SERVO_NECK_YAW_IDX   (15)
#define SERVO_NECK_PTICH_IDX (14)
#define SERVO_EYE_LEFT_IDX   (13)
#define SERVO_EYE_RIGHT_IDX  (12)

// Uncomment the following line to reverse the direction of the track motor
// #define MOTOR_RIGHT_REVERSE_DIRECTION
// #define MOTOR_LEFT_REVERSE_DIRECTION

/*---- Audio Player Settings -------------------------------------------
*  Various settings of the Audio Player module.
*  -------------------------------------------------------------------*/
// #define ENABLE_AUDIO // Comment this line to disable audio player
#define DEFAULT_AUDIO_VOLUME (24)
#define STARTUP_TRACK_INDEX (1) // Set to 0 if you don't want to play anything

#endif /* CONFIG_HPP */
