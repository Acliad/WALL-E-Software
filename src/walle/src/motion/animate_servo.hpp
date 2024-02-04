/**
 * @file animate_servo.hpp
 * @author Isaac Rex (@Acliad)
 * @brief A class for animating WALL-E's head. This class is intended to be used with the ServoKeyframe class. Provides
 * an interface for linking keyframes together and playing them back.
 * @version 0.1
 * @date 2023-10-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ANIMATE_SERVO_HPP
#define ANIMATE_SERVO_HPP

#include "servo_keyframe.hpp"
#include <Arduino.h>
#include <Ramp.h>

class ServoAnimation {
  public:
    ServoAnimation();
    ~ServoAnimation();

    void add_keyframe(ServoKeyframe *keyframe);
    void play();
    void stop();
    void update();

    bool isPlaying();

  private:
    ServoKeyframe *_head;
    ServoKeyframe *_current_keyframe;
    unsigned long _frame_start_time_ms;
    bool          _playing;
    bool          _keyframe_has_started;
};

#endif // ANIMATE_SERVO_HPP