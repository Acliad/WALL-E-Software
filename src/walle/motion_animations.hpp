#ifndef MOTION_ANIMATIONS_HPP
#define MOTION_ANIMATIONS_HPP

#include "src/motion/servo_context.hpp"
#include "src/motion/animate_servo.hpp"


namespace MotionAnimations {
    extern ServoAnimation cock_left;
    extern ServoAnimation cock_right;
    extern ServoAnimation sad;
    extern ServoAnimation curious_track;
    extern ServoAnimation wiggle_eyes;
    
    void setup_animations(ServoContext servos);
}

#endif // MOTION_ANIMATIONS_HPP
