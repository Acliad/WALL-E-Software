/**
 * @file drive_motor.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the declaration of the DriveMotor class, which is used to control a track motor.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef DRIVE_MOTOR_HPP
#define DRIVE_MOTOR_HPP

#define _DRIVE_MOTOR_S_TO_MS (1000)
#define _DRIVE_MOTOR_DEFAULT_ACCELERATION (10.0 / _DRIVE_MOTOR_S_TO_MS)

#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>


/**
 * @brief Class representing a drive motor.
 * TODO: Convert this to inherit from Motor
 * This class provides methods to control the speed and parameters of a drive motor.
 */
class DriveMotor {
  public:
    /**
     * @brief Constructs a DriveMotor object. 
     *
     * NOTE: Neutral is assumed to be the center of min_us and max_us. Motor is set to neutral on construction. Does not
     * call Adafruit_PWMServoDriver's begin() for you, please begin before using. This is to allow for multiple motors
     * to be controlled by the same PCA9685.
     * 
     * @param pca9685 Pointer to the Adafruit_PWMServoDriver object used to control the motor.
     * @param pin The pin number of the motor.
     * @param min_us The minimum pulse width in microseconds for the motor.
     * @param max_us The maximum pulse width in microseconds for the motor.
     */
    DriveMotor(Adafruit_PWMServoDriver *pca9685, int pin, int min_us = 500, int max_us = 2500);

    /**
     * @brief Sets the speed of the motor.
     * 
     * @param speed The speed of the motor as a scaler value from -1.0 to 1.0.
     */
    void set_speed(float speed);

    /**
     * @brief Sets a scaling factor for the maximum speed as a ratio of the maximum capable speed. This remaps the speed
     * range from -max_speed to max_speed such that set_speed(1.0) will set the motor to max_speed and set_speed(-1.0)
     * will set the motor to -max_speed.
     *
     * @param max_speed The maximum speed scaling factor.
     */
    void set_speed_limit(float max_speed);

    /**
     * @brief Sets the acceleration value for the motor.
     * 
     * @param acceleration_per_ss The acceleration value in units of speed change per second squared.
     */
    void set_acceleration(float acceleration_per_ss);

    /**
     * @brief Sets the deceleration value for the motor.
     * 
     * @param deceleration_per_ss The deceleration value in units of speed change per second squared.
     */
    void set_deceleration(float deceleration_per_ss);

    /**
     * @brief Updates the motor speed and writes the output to the PWM driver. This method should be called periodically
     * to update the motor speed.
     * 
     * @return The current speed of the motor.
     */
    float update();

    /**
     * @brief Gets the current speed of the motor.
     * 
     * @return The current speed of the motor.
     */
    float get_current_speed();

    /**
     * @brief Gets the speed limit of the motor.
     * 
     * @return The speed limit of the motor.
     */
    float get_speed_limit();

  private:
    Adafruit_PWMServoDriver *_pwm_driver;
    int                      _pin;
    int                      _min_us;
    int                      _max_us;
    int                      _neutral_us;
    unsigned long            _last_update_ms;
    float _acceleration_per_ms;
    float _deceleration_per_ms;
    float _current_speed;
    float _target_speed;
    float _max_speed;

    /**
     * @brief Calculates the current speed for the constant acceleration type.
     */
    void _update_speed_constant_accel();

    /**
     * @brief Converts a speed value to pulse width in microseconds.
     * 
     * @param speed The speed value to convert.
     * @return The pulse width in microseconds.
     */
    unsigned int _speed_to_us(float speed);

    /**
     * @brief Converts a pulse width in microseconds to a speed value.
     * 
     * @param us The pulse width in microseconds.
     * @return The speed value.
     */
    float _us_to_speed(int us);
};

#endif // DRIVE_MOTOR_HPP