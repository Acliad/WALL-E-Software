#include "solar_animations.hpp"

AnimateLevel startup_animation = AnimateLevel();

void setup_startup_animation() {
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
    startup_animation.addKeyframe(0, 80, false);

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
    startup_animation.addKeyframe(0, 1000, true);


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
    startup_animation.addKeyframe(2, 1000);
    startup_animation.addPauseKeyframe(800);

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
    startup_animation.addKeyframe(10, 900);
}

void setup_animations() {
    // This function should get called in the main setup() function. It calls all animation setup functions for use in
    // the main sketch.
    setup_startup_animation();
}