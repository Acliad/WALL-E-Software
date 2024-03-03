/**
 * @file motion_animations.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the declaration of the MotionAnimations namespace, which is used to define and setup
 * animations for the servo motors. See readme.md for more information on defining animations.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef MOTION_ANIMATIONS_HPP
#define MOTION_ANIMATIONS_HPP

#include "src/motion/servo_context.hpp"
#include "src/motion/animate_servo.hpp"


namespace MotionAnimations {
    // Add custom animations here:
    
    extern ServoAnimation cock_left;
    extern ServoAnimation cock_right;
    extern ServoAnimation sad;
    extern ServoAnimation curious_track;
    extern ServoAnimation wiggle_eyes;
    
    void setup_animations(ServoContext servos);
}

#endif // MOTION_ANIMATIONS_HPP
