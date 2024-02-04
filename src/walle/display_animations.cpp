#include "display_animations.hpp"
#include "src/display/animate_level.hpp"
#include "src/motion/animate_servo.hpp"

// Define new animations here

namespace DisplayAnimations {
AnimateLevel startup = AnimateLevel();
// Create setup functions for each animation.
// NOTE: Don't forget to call these functions in setup_animations().
void setup_startup() {
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
    startup.addKeyframe(0, 1000, true);

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

void setup_animations() {
    // This function should get called in the main setup() function. It calls all animation setup functions for use in
    // the main sketch.
    setup_startup();
}
} // namespace DisplayAnimations