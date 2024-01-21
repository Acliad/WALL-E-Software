/**
 * @file servoMotor.hpp
 * @author Isaac Rex (@Acliad)
 * @brief Provides an inteface for controlling WALL-E's servo motors. This library allows the caller to specify a
 * certain curve and time for moving the servo and the library will control the motor's control signals accordingly.
 * @version 0.1
 * @date 2023-10-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SERVO_MOTOR_HPP
#define SERVO_MOTOR_HPP

#include "motor.hpp"
#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Ramp.h>

class ServoMotor : public Motor {
    // NOTE: Motor is set to neutral on construction. Does not call Adafruit_PWMServoDriver's begin() for you, please
    // begin before using. This is to allow for multiple motors to be controlled by the same PCA9685.
  public:
    ServoMotor(Adafruit_PWMServoDriver *pca9685, int pin, int neutral_us = 1500, int min_us = 500, int max_us = 2500,
               float min_angle_deg = -90, float max_angle_deg = 90, float neutral_angle_deg = 0);

    // Set the angle in degrees from -90 to 90
    void set_angle(float angle, unsigned long time_ms = 0);
    void set_min_angle(float min_angle_deg);
    void set_max_angle(float max_angle_deg);
    void set_neutral_angle(float neutral_angle_deg);
    void set_min_us(int min_us);
    void set_max_us(int max_us);
    void set_neutral_us(int neutral_us);
    void set_scalar(float scalar, unsigned long time_ms);
    void set_ramp_mode(ramp_mode mode);

    void update();

    float get_angle();
    float angle_to_scalar(float angle_deg);
    float us_to_angle(int us);
    float angle_to_us(float angle_deg);

  private:
    float        _min_angle_deg;
    float        _max_angle_deg;
    float        _neutral_angle_deg;
    float        _angle_positive_to_us_slope;
    float        _angle_negative_to_us_slope;
    unsigned int _target_us;
    rampInt      _us_ramp;
    ramp_mode    _ramp_mode;
};

#endif // SERVO_MOTOR_HPP