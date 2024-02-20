/**
 * @file display_animations.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the definition of the DisplayAnimations namespace, which is used to define the animations
 * that are used in the display module.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef DISPLAY_ANIMATIONS_HPP
#define DISPLAY_ANIMATIONS_HPP
#include "src/display/animate_solar_panel.hpp"
#include "src/motion/animate_servo.hpp"
#include "config.hpp"
#include "src/audio/audio_player.hpp"

namespace DisplayAnimations {
    extern AnimateSolarPanel startup;
    void setup_animations(DfMp3 *dfmp3);
}

#endif // DISPLAY_ANIMATIONS_HPP