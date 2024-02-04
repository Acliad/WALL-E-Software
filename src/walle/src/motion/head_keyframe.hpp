/**
 * @file head_keyframe.hpp
 * @author Isaac Rex (@Acliad)
 * @brief A class for keyframes in head animations. This class is intended to be used with the HeadAnimation class.
 * Provides an interface for setting keyframes as servo positions, ramp curve, and duration of the keyframe.
 * @version 0.1
 * @date 2023-10-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef HEAD_KEYFRAME_HPP
#define HEAD_KEYFRAME_HPP

#include "servo_motor.hpp"
#include <Arduino.h>
#include <Ramp.h>

class HeadKeyframe {
  public:
    HeadKeyframe(unsigned long duration_ms);
    ~HeadKeyframe();

    void add_servo_angle(ServoMotor *servo, float angle, ramp_mode ramp_mode = QUADRATIC_INOUT);
    void add_servo_scalar(ServoMotor *servo, float scalar, ramp_mode ramp_mode = QUADRATIC_INOUT);

    // Sets up the Ramp for all the servos in this keyframe and starts them
    void start_keyframe();
    // Update the internal Ramp state for all the servos in this keyframe
    void update_keyframe();

    void set_next(HeadKeyframe *next);
    void set_prev(HeadKeyframe *prev);

    unsigned long get_duration_ms();

    HeadKeyframe *get_next();
    HeadKeyframe *get_prev();

  private:
    struct servo_keyframe {
        ServoMotor     *_servo;
        float           _target_scalar;
        ramp_mode       _ramp_mode;
        servo_keyframe *_next;
    };
    unsigned long _duration_ms;

    HeadKeyframe *_next;
    HeadKeyframe *_prev;

    servo_keyframe *_head;
};

#endif // KEYFRAME_HEAD_HPP