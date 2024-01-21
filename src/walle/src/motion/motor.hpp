/**
 * @file motor.hpp
 * @author Isaac Rex (@Acliad)
 * @brief A superclass for PWM controlled motors. Provides an interface for controlling WALL-E's motors. This library
 * provides basic functions for the drive motors and servo motors. Everything in this class is relative to us.
 * Subclasses Should map us to their own units.
 * @version 0.1
 * @date 2023-10-11
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
    // NOTE: Neutral is assumed to be the center of min_us and max_us. Motor is set to neutral on construction (but
    // won't be updated until update() is called). Does not call Adafruit_PWMServoDriver's begin() for you, please begin
    // before using. This is to allow for multiple motors to be controlled by the same PCA9685.
    Motor(Adafruit_PWMServoDriver *pca9685, int pin, int neutral_us = 1500, int min_us = 500, int max_us = 2500);

    virtual void update();

    // Set the us value for the motor
    void set_us(int us);

    // Set the speed as a scaler from -1.0 to 1.0. -1.0 is min_us, 1.0 is max_us.
    void set_scalar(float scalar);

    virtual void set_min_us(int min_us);
    virtual void set_max_us(int max_us);
    virtual void set_neutral_us(int neutral_us);

    int   get_current_us();
    float get_current_scalar();

  protected:
    Adafruit_PWMServoDriver *_pwm_driver;
    int                      _pin;
    int                      _min_us;
    int                      _max_us;
    int                      _neutral_us;
    // Used to track parameters for asymetric mapping (i.e., _neutral_us - _min_us != _max_us - _neutral_us)
    float         _scalar_plus_to_us_slope;
    float         _scalar_minus_to_us_slope;
    int           _current_us;
    unsigned long _last_update_ms;
    // Helper functions for converting between us and speed

    int   _scalar_to_us(float speed);
    float _us_to_scaler(int us);
};

#endif // MOTOR_HPP