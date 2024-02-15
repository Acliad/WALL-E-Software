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

#include <FS.h>
#include <SPIFFS.h>
#include <Arduino.h>
#include <Ramp.h>
#include <sstream>
#include <string>
#include <iostream>
#include "servo_keyframe.hpp"
#include "servo_context.hpp"

class ServoAnimation {
  public:
    ServoAnimation();
    ~ServoAnimation();

    void add_keyframe(ServoKeyframe *keyframe);
    void play();
    void stop();
    void update();

    bool isPlaying();

    bool save(fs::FS &filesystem, const char* filename);
    static std::unique_ptr<ServoAnimation> load(fs::FS &filesystem, const char* filename, ServoContext& servo_context);

    void printDebugInfo();

  private:
    ServoKeyframe *_head;
    ServoKeyframe *_current_keyframe;
    unsigned long _frame_start_time_ms;
    bool          _playing;
    bool          _keyframe_has_started;

    static constexpr char* const _SERIALIZED_KEYFRAME_START = "start keyframe";
    static constexpr char* const _SERIALIZED_KEYFRAME_END = "end keyframe";
};

#endif // ANIMATE_SERVO_HPP