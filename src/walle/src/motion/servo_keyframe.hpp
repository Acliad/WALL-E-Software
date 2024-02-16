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

#include <sstream>
#include <string>
#include <iostream>
#include <Arduino.h>
#include <Ramp.h>
#include "servo_motor.hpp"
#include "servo_context.hpp"
#include "../audio/audio_player.h"

class ServoKeyframe {
  public:
    ServoKeyframe(unsigned long duration_ms);
    ServoKeyframe(const ServoKeyframe &keyframe);
    ~ServoKeyframe();

    void add_servo_angle(ServoMotor *servo, float angle, ramp_mode ramp_mode = QUADRATIC_INOUT);
    void add_servo_scalar(ServoMotor *servo, float scalar, ramp_mode ramp_mode = QUADRATIC_INOUT);

    // Add a track to play at the start of the keyframe
    void add_track(int track_index, DfMp3 *dfmp3);

    // Sets up the Ramp for all the servos in this keyframe and starts them
    void start_keyframe();
    // Update the internal Ramp state for all the servos in this keyframe
    void update_keyframe();
    
    void set_duration(unsigned long duration_ms);
    unsigned long get_duration();

    // Setters and getters for the linked list of keyframes
    void set_next(ServoKeyframe *next);
    ServoKeyframe *get_next();

    void set_prev(ServoKeyframe *prev);
    ServoKeyframe *get_prev();

    std::string serialize() const;
    static ServoKeyframe *deserialize(std::string keyframe_string, ServoContext& servo_context);

    // Deubgging function to print the servos in the keyframe
    void print_servos() const;

  private:
    struct servo_node {
        ServoMotor *_servo;
        float       _target_scalar;
        ramp_mode   _ramp_mode;
        servo_node *_next;
    };
    servo_node *_servo_head;

    unsigned long _duration_ms;

    ServoKeyframe *_next;
    ServoKeyframe *_prev;

    int _track_index;
    bool _track_has_played;
    DfMp3 *_dfmp3;
};

#endif // SERVO_KEYFRAME_HPP