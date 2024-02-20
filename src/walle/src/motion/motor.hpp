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

/**
 * @brief Represents a motor controlled by a PCA9685 PWM servo driver.
 */
class Motor {
  public:
    /**
     * @brief Constructs a Motor object.  
     *
     * NOTE: Motor is set to neutral on construction (but won't be updated until update() is called). Does not call
     * Adafruit_PWMServoDriver's begin() for you, please begin before using. This is to allow for multiple motors to be
     * controlled by the same PCA9685.
     *
     * @param pca9685 Pointer to the PCA9685 PWM servo driver.
     * @param pin The pin number of the motor.
     * @param name The name of the motor.
     * @param neutral_us The neutral position's pulse width in microseconds (default: 1500).
     * @param min_us The minimum pulse width in microseconds (default: 500).
     * @param max_us The maximum pulse width in microseconds (default: 2500).
     */
    Motor(Adafruit_PWMServoDriver *pca9685, int pin, std::string name, int neutral_us = 1500, int min_us = 500,
          int max_us = 2500);

    /**
     * @brief Updates the motor's position based on the current pulse width.
     */
    virtual void update();

    /**
     * @brief Sets the pulse width of the motor in microseconds.
     * 
     * @param us The pulse width in microseconds.
     */
    void set_us(int us);

    /**
     * @brief Sets the value of the motor as a scalar value from -1.0 to 1.0. 0 corresponds to the neutral position and 
     * 0 to -1.0 is linearly mapped to the min_us to neutral_us range and 0 to 1.0 is linearly mapped to the neutral_us
     * to max_us range.
     * 
     * @param scalar The value to set (-1.0 to 1.0).
     */
    void set_scalar(float scalar);

    /**
     * @brief Sets the minimum pulse width of the motor in microseconds.
     * 
     * @param min_us The minimum pulse width in microseconds.
     */
    virtual void set_min_us(int min_us);

    /**
     * @brief Sets the maximum pulse width of the motor in microseconds.
     * 
     * @param max_us The maximum pulse width in microseconds.
     */
    virtual void set_max_us(int max_us);

    /**
     * @brief Sets the neutral pulse width of the motor in microseconds.
     * 
     * @param neutral_us The neutral pulse width in microseconds.
     */
    virtual void set_neutral_us(int neutral_us);

    /**
     * @brief Gets the current pulse width of the motor in microseconds.
     * 
     * @return The current pulse width in microseconds.
     */
    int get_current_us();

    /**
     * @brief Gets the current speed scalar value of the motor.
     * 
     * @return The current speed scalar value.
     */
    float get_current_scalar();

    /**
     * @brief Gets the name of the motor.
     * 
     * @return The name of the motor.
     */
    std::string get_name();

  protected:
    Adafruit_PWMServoDriver *_pwm_driver; // Pointer to the PCA9685 PWM servo driver
    int                      _pin; // The pin number of the motor
    int                      _min_us; // The minimum pulse width in microseconds
    int                      _max_us; // The maximum pulse width in microseconds
    int                      _neutral_us; // The neutral pulse width in microseconds
    float         _scalar_plus_to_us_slope; // Slope for converting speed scalar to pulse width (positive range)
    float         _scalar_minus_to_us_slope; // Slope for converting speed scalar to pulse width (negative range)
    int           _current_us; // The current pulse width in microseconds
    unsigned long _last_update_ms; // The timestamp of the last update
    std::string   _name; // The name of the motor

    /**
     * @brief Converts the speed scalar value to pulse width in microseconds.
     * 
     * @param speed The speed scalar value.
     * @return The pulse width in microseconds.
     */
    int _scalar_to_us(float speed);

    /**
     * @brief Converts the pulse width in microseconds to speed scalar value.
     * 
     * @param us The pulse width in microseconds.
     * @return The speed scalar value.
     */
    float _us_to_scaler(int us);
};

#endif // MOTOR_HPP