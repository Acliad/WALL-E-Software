/**
 * @file head_animation.hpp
 * @author Isaac Rex (@Acliad)
 * @brief A class for animating WALL-E's head. This class is intended to be used with the HeadKeyframe class. Provides
 * an interface for linking keyframes together and playing them back.
 * @version 0.1
 * @date 2023-10-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef HEAD_ANIMATION_HPP
#define HEAD_ANIMATION_HPP

#include "head_keyframe.hpp"
#include <Arduino.h>
#include <Ramp.h>

class HeadAnimation {
  public:
    HeadAnimation();

    void add_keyframe(HeadKeyframe *keyframe);
    void play();
    void stop();
    void update();

  private:
    HeadKeyframe *_head;
    HeadKeyframe *_current_keyframe;
    unsigned long _frame_start_time_ms;
    bool          _playing;
    bool          _keyframe_has_started;
};

#endif // HEAD_ANIMATION_HPP