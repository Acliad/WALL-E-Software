/**
 * @file motor.hpp
 * @author Isaac Rex (@Acliad)
 * @brief A superclass for PWM controlled motors. Provides an interface for controlling WALL-E's motors. This library
 * provides basic functions for the drive motors and servo motors. 
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
    // NOTE: Neutral is assumed to be the center of min_us and max_us. Motor is set to neutral on construction. Does not
    // call Adafruit_PWMServoDriver's begin() for you, please begin before using. This is to allow for multiple motors
    // to be controlled by the same PCA9685.
    Motor(Adafruit_PWMServoDriver *pca9685, int pin, int min_us = 500, int max_us = 2500);

    virtual float update();

    // Set the us value for the motor
    void set_us(int us);

    // Set the speed as a scaller from 0.0 to 1.0 (not technically percent). 0.0 is min_us, 1.0 is max_us.
    void set_percent(float percent);

    void set_min_us(int min_us);
    void set_max_us(int max_us);

    float get_current_us();


  protected:
    Adafruit_PWMServoDriver *_pwm_driver;
    int                      _pin;
    int                      _min_us;
    int                      _max_us;
    int                      _neutral_us;
    unsigned long            _last_update_ms;
    // Helper functions for converting between us and speed

    int   _percent_to_us(float speed);
    float _us_to_percent(int us);
};



#endif // MOTOR_HPP