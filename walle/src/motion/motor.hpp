/**
 * @file motor.hpp
 * @author Isaac Rex (@Acliad)
 * @brief Provides a standardized interface for controlling WALL-E's drive motors and servors. Meant to be used with
 * the motion library.
 * @version 0.1
 * @date 2023-10-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>


class Motor {
  public:
    // NOTE: Neutral is assumed to be the center of min_us and max_us. Motor is set to neutral on construction. Does not
    // call Adafruit_PWMServoDriver's begin() for you, please begin before using. This is to allow for multiple motors
    // to be controlled by the same PCA9685.
    Motor(Adafruit_PWMServoDriver *pca9685, int pin, int min_us = 500, int max_us = 2500,
          int min_angle_deg = -90, int max_angle_deg = 90, float _max_speed = 1.0);


    // Directly set the output pulse width in microseconds
    void set_us(int us);
    // Set the speed as a scaller from -1.0 to 1.0
    void set_speed(float speed);
    // Set the angle in degrees from -90 to 90
    void set_angle(float angle);
    // Sets a scaling factor for the max speed. set_speed_limit(0.5); set_speed(1.0) will result in a speed of 0.5.
    void set_speed_limit(float max_speed);
    // Call this in your main loop. Updates calculated speed for acceleration curves and writes the output to the
    // pwm_driver. Returns the current speed.
    float update();

    // Getters for the above setters
    int   get_us();
    float get_speed();
    float get_angle();

    float us_to_speed(int us);
    float us_to_angle(int us);
  private:
    Adafruit_PWMServoDriver *_pwm_driver;
    int                      _pin;
    int                      _min_us;
    int                      _max_us;
    int                      _neutral_us;
    int                      _current_us;
    int                      _min_angle_deg;
    int                      _max_angle_deg;
    float                    _angle_to_us_slope;
    float                    _angle_to_us_intercept;
    float                    _max_speed;
};

#endif // MOTOR_HPP