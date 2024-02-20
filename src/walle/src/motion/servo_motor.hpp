/**
 * @file servoMotor.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the declaration of the ServoMotor class, which is used to control a servo motor. This
 * library allows the caller to specify a certain curve and time for moving the servo and the library will control the
 * motor's control signals accordingly.
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

/**
 * @brief Represents a servo motor that extends the Motor class.
 * 
 * This class provides methods to control the angle and movement of a servo motor.
 */
class ServoMotor : public Motor {
  public:
    /**
     * @brief Constructs a ServoMotor object.
     *
     * NOTE: Motor is set to neutral on construction. Does not call Adafruit_PWMServoDriver's begin() for you, please
     * begin before using. This is to allow for multiple motors to be controlled by the same PCA9685.
     * 
     * TODO: The min/max setters should probably be made limmiters rather than remappers. What was I thinking?
     *
     * @param pca9685 Pointer to the Adafruit_PWMServoDriver object.
     * @param pin The pin number of the servo motor.
     * @param name The name of the servo motor.
     * @param neutral_us The neutral pulse width in microseconds (default: 1500).
     * @param min_us The minimum pulse width in microseconds (default: 500).
     * @param max_us The maximum pulse width in microseconds (default: 2500).
     * @param min_angle_deg The minimum angle in degrees (default: -90).
     * @param max_angle_deg The maximum angle in degrees (default: 90).
     * @param neutral_angle_deg The neutral angle in degrees (default: 0).
     */
    ServoMotor(Adafruit_PWMServoDriver *pca9685, int pin, std::string name, int neutral_us = 1500, int min_us = 500, int max_us = 2500,
               float min_angle_deg = -90, float max_angle_deg = 90, float neutral_angle_deg = 0);

    /**
     * @brief Sets the angle of the servo motor.
     * 
     * @param angle The angle in degrees (-90 to 90).
     * @param time_ms The time in milliseconds for the servo motor to reach the target angle (default: 0).
     */
    void set_angle(float angle, unsigned long time_ms = 0);

    /**
     * @brief Sets the minimum physical angle of the servo motor. This remaps min_us to min_angle_deg.
     * 
     * @param min_angle_deg The minimum angle in degrees.
     */
    void set_min_angle(float min_angle_deg);

    /**
     * @brief Sets the maximum angle of the servo motor. This remaps max_us to max_angle_deg.
     * 
     * @param max_angle_deg The maximum angle in degrees.
     */
    void set_max_angle(float max_angle_deg);

    /**
     * @brief Sets the neutral angle of the servo motor. This remaps neutral_us to neutral_angle_deg.
     * 
     * @param neutral_angle_deg The neutral angle in degrees.
     */
    void set_neutral_angle(float neutral_angle_deg);

    /**
     * @brief Sets the minimum pulse width of the servo motor.
     * 
     * @param min_us The minimum pulse width in microseconds.
     */
    void set_min_us(int min_us);

    /**
     * @brief Sets the maximum pulse width of the servo motor.
     * 
     * @param max_us The maximum pulse width in microseconds.
     */
    void set_max_us(int max_us);

    /**
     * @brief Sets the neutral pulse width of the servo motor.
     * 
     * @param neutral_us The neutral pulse width in microseconds.
     */
    void set_neutral_us(int neutral_us);

    /**
     * @brief Sets the scalar value for the servo motor.
     * 
     * @param scalar The scalar value. -1.0 is min_us, 1.0 is max_us.
     * @param time_ms The time in milliseconds for the servo motor to reach the target scalar value.
     */
    void set_scalar(float scalar, unsigned long time_ms);

    /**
     * @brief Sets the ramp mode for the servo motor.
     * 
     * @param mode The ramp mode.
     */
    void set_ramp_mode(ramp_mode mode);

    /**
     * @brief Updates the servo motor.
     * 
     * This method should be called periodically to update the servo motor's position.
     */
    void update();

    /**
     * @brief Gets the current angle of the servo motor.
     * 
     * @return The current angle in degrees.
     */
    float get_angle();

    /**
     * @brief Gets the current scalar value of the servo motor.
     * 
     * @return The current scalar value. -1.0 is min_us, 1.0 is max_us.
     */
    float get_scalar();

    /**
     * @brief Converts an angle in degrees to a scalar value.
     * 
     * @param angle_deg The angle in degrees.
     * @return The scalar value.
     */
    float angle_to_scalar(float angle_deg);

    /**
     * @brief Converts a pulse width in microseconds to an angle in degrees.
     * 
     * @param us The pulse width in microseconds.
     * @return The angle in degrees.
     */
    float us_to_angle(int us);

    /**
     * @brief Converts a pulse width in microseconds to a scalar value.
     * 
     * @param us The pulse width in microseconds.
     * @return The scalar value.
     */
    float us_to_scalar(int us);

    /**
     * @brief Converts an angle in degrees to a pulse width in microseconds.
     * 
     * @param angle_deg The angle in degrees.
     * @return The pulse width in microseconds.
     */
    float angle_to_us(float angle_deg);

    /**
     * @brief Prints debug information about the servo motor.
     * 
     * This method is used for debugging purposes.
     */
    void print_debug();

  private:
    float        _min_angle_deg;              // The minimum angle in degrees
    float        _max_angle_deg;              // The maximum angle in degrees
    float        _neutral_angle_deg;          // The neutral angle in degrees
    float        _angle_positive_to_us_slope; // The slope for converting positive angles to pulse widths
    float        _angle_negative_to_us_slope; // The slope for converting negative angles to pulse widths
    unsigned int _target_us;                  // The target pulse width in microseconds
    rampInt      _us_ramp;                    // The ramp for the pulse width
    ramp_mode    _ramp_mode;                  // The ramp mode
};

#endif // SERVO_MOTOR_HPP