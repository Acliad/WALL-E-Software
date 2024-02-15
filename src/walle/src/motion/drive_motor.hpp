/**
 * @file drive_motor.hpp
 * @author Isaac Rex (@Acliad)
 * @brief Provides an inteface for controlling WALL-E's track drive motors. This library allows the caller to specify an
 * accerlation value and the library will control the motor's control signals accordingly.
 * @version 0.1
 * @date 2023-10-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef DRIVE_MOTOR_HPP
#define DRIVE_MOTOR_HPP

#define _DRIVE_MOTOR_S_TO_MS (1000)
#define _DRIVE_MOTOR_DEFAULT_ACCELERATION (10.0 / _DRIVE_MOTOR_S_TO_MS)

#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>

// TODO: Conver this to inherit from Motor
class DriveMotor {
  public:
    // NOTE: Neutral is assumed to be the center of min_us and max_us. Motor is set to neutral on construction. Does not
    // call Adafruit_PWMServoDriver's begin() for you, please begin before using. This is to allow for multiple motors
    // to be controlled by the same PCA9685.
    DriveMotor(Adafruit_PWMServoDriver *pca9685, int pin, int min_us = 500, int max_us = 2500);

    // Set the speed as a scaller from -1.0 to 1.0
    void set_speed(float speed);

    // Sets a scaling factor for the max speed. set_speed_limit(0.5); set_speed(1.0) will result in a speed of 0.5.
    void set_speed_limit(float max_speed);
    // Setters for parameters
    void set_acceleration(float acceleration_per_ss);
    void set_deceleration(float deceleration_per_ss);
    // Call this in your main loop. Updates calculated speed for acceleration curves and writes the output to the
    // pwm_driver. Returns the current speed.
    float update();

    float get_current_speed();
    float get_speed_limit();

  private:
    Adafruit_PWMServoDriver *_pwm_driver;
    int                      _pin;
    int                      _min_us;
    int                      _max_us;
    int                      _neutral_us;
    unsigned long            _last_update_ms;
    // Store acceleration per ms^2 instead of per s^2 so we don't have to do conversions every time we update
    float _acceleration_per_ms;
    float _deceleration_per_ms;
    float _current_speed;
    float _target_speed;
    float _max_speed;

    // Calculates the current speed for the acceleration type
    void _update_speed_constant_accel();
    // Helper functions for converting between us and speed
    unsigned int _speed_to_us(float speed);
    float        _us_to_speed(int us);
};

#endif // DRIVE_MOTOR_HPP