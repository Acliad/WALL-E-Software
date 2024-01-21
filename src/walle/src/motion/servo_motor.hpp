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


#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>

class ServoMotor {
    // NOTE: Neutral is assumed to be the center of min_us and max_us. Motor is set to neutral on construction. Does not
    // call Adafruit_PWMServoDriver's begin() for you, please begin before using. This is to allow for multiple motors
    // to be controlled by the same PCA9685.
  public:
    ServoMotor(Adafruit_PWMServoDriver *pca9685, int pin, int min_us = 500, int max_us = 2500, int min_angle_deg = -90,
               int max_angle_deg = 90);

    // Set the angle in degrees from -90 to 90
    void  set_angle(float angle);
    float get_angle();

    float us_to_angle(int us);

  private:
    int   _min_angle_deg;
    int   _max_angle_deg;
    float _angle_to_us_slope;
    float _angle_to_us_intercept;
};

#endif // SERVO_MOTOR_HPP