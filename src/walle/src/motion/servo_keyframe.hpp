/**
 * @file servo_keyframe.hpp
 * @author Isaac Rex (@Acliad)
 * @brief A class for keyframes in head animations. This class is intended to be used with the ServoAnimation class.
 * Provides an interface for setting keyframes as servo positions, ramp curve, and duration of the keyframe.
 * @version 0.1
 * @date 2023-10-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SERVO_KEYFRAME_HPP
#define SERVO_KEYFRAME_HPP

#include "servo_motor.hpp"
#include <Arduino.h>
#include <Ramp.h>

class ServoKeyframe {
  public:
    ServoKeyframe(unsigned long duration_ms);
    ~ServoKeyframe();

    void add_servo_angle(ServoMotor *servo, float angle, ramp_mode ramp_mode = QUADRATIC_INOUT);
    void add_servo_scalar(ServoMotor *servo, float scalar, ramp_mode ramp_mode = QUADRATIC_INOUT);

    // Sets up the Ramp for all the servos in this keyframe and starts them
    void start_keyframe();
    // Update the internal Ramp state for all the servos in this keyframe
    void update_keyframe();

    void set_next(ServoKeyframe *next);
    void set_prev(ServoKeyframe *prev);

    unsigned long get_duration_ms();

    ServoKeyframe *get_next();
    ServoKeyframe *get_prev();

  private:
    struct servo_node {
        ServoMotor     *_servo;
        float           _target_scalar;
        ramp_mode       _ramp_mode;
        servo_node *_next;
    };
    unsigned long _duration_ms;

    ServoKeyframe *_next;
    ServoKeyframe *_prev;

    servo_node *_head;
};

#endif // SERVO_KEYFRAME_HPP