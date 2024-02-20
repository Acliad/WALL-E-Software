/**
 * @file display_animations.cpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the implementation of the display animations. This file is included in the main sketch to
 * provide the animations for the display module. See readme.md for more information on how to create new animations.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "display_animations.hpp"


// Define new animations here

namespace DisplayAnimations {
AnimateSolarPanel startup = AnimateSolarPanel();
// Create setup functions for each animation.
// NOTE: Don't forget to call these functions in setup_animations().
void setup_startup(DfMp3 *dfmp3) {
    // Setup the keyframe to animate. We want to turn on the first 5 bars over 3000 ms, pause for 1000 ms,
    // Setup the keyframe to animate. We want to turn on the first 5 bars over 3000 ms, pause for 1000 ms, then turn on
    // the rest of the bars over 1000 ms.
    // Create the first keyframe
    // |____________|
    // |            |
    // |            |
    // |            |
    // |            |
    // |            |
    // |            |
    // |            |
    // |            |
    // |            |
    startup.addKeyframe(0, 80, false);

    // Turn on the sun for the first beep
    //             |____________|
    //   \ | /     |            |
    // - ( . ) -   |            |
    //   / | \     |            |
    //             |            |
    //             |            |
    //             |            |
    //             |            |
    //             |            |
    //             |            |
    startup.addKeyframe(0, 1000, TRACK_INDEX_STARTUP, dfmp3, true);

    // |____________|
    // |            |
    // |            |
    // |            |
    // |            |
    // |            |
    // |            |
    // |            |
    // |            |
    // |████████████|
    // |████████████|
    startup.addKeyframe(2, 1000);
    startup.addPauseKeyframe(800);

    // |____________|
    // |████████████|
    // |████████████|
    // |████████████|
    // |████████████|
    // |████████████|
    // |████████████|
    // |████████████|
    // |████████████|
    // |████████████|
    // |████████████|
    startup.addKeyframe(10, 900);
}

void setup_animations(DfMp3 *dfmp3) {
    // This function should get called in the main setup() function. It calls all animation setup functions for use in
    // the main sketch.
    setup_startup(dfmp3);
}
} // namespace DisplayAnimations