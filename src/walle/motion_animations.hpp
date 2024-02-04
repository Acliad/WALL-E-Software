#ifndef MOTION_ANIMATIONS_HPP
#define MOTION_ANIMATIONS_HPP

#include "contexts.hpp"
#include "src/motion/head_animation.hpp"


namespace MotionAnimations {
    extern HeadAnimation cock_left;
    extern HeadAnimation cock_right;
    extern HeadAnimation sad;
    extern HeadAnimation curious_track;
    extern HeadAnimation wiggle_eyes;
    
    void setup_animations(ServoContext servos);
}

#endif // MOTION_ANIMATIONS_HPP
