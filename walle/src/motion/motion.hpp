/**
 * @file motion.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This library provides functions for controlling the motion of the drive motors for WALL-E. It allows the
 * caller to specify a certain kind of acceleration curve for the motors and controls the motor's control signals
 * accordingly.
 *
 * @version 0.1
 * @date 2023-10-01
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MOTION_HPP
#define MOTION_HPP

#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include <Ramp.h>
#include "motor.hpp"

class Motion {
  public:
    Motion(Motor *motor, ramp_mode ramp_mode = EXPONENTIAL_INOUT, float acceleration_per_ss = 10,
           float deceleration_per_ss = 10, float max_speed = 1.0, float min_speed = -1.0);

    void begin();
    void set_speed(float speed);
    void update();

    // Setters for parameters
    void set_ramp_mode(ramp_mode ramp_mode);
    void set_acceleration(float acceleration_per_ss);
    void set_deceleration(float deceleration_per_ss);
    // Sets a scaling factor for the max speed. set_speed_limit(0.5); set_speed(1.0) will result in a speed of 0.5.
    void set_speed_limit(float max_speed);

  private:
    Motor    *_motor;
    rampFloat _ramp;
    ramp_mode _ramp_mode;
    // Store acceleration per ms^2 instead of per s^2 so we don't have to do conversions every time we update
    float     _acceleration_per_ms;
    float     _deceleration_per_ms;
    float     _current_speed;
    float     _max_speed;

    unsigned long last_update_time_ms;

    // Calculates the current speed for the acceleration type
    float _get_speed_constant_accel(unsigned long dt_ms);
};

#endif // MOTION_HPP