/**
 * @file animate_solar_panel.hpp
 * @brief This file defines the animation class for the solar bars.
 *
 * This file defines the animation class for the solar bars. This class is used to create animations for the solar bars.
 * The animation class is used to create a linked list of keyframes. Each keyframe contains an array of bar statuses and
 * a duration. A keyframe will move the number of bars from the previous keyframe to the current in a linear fashion.
 *
 * For example:
 *  - keyframe 3 has 3 bars
 *  - on keyframe 4 has 5 bars on
 *  - duration is 1000 ms
 * Animating from keyframe 3 to keyframe 4 will turn on bar 4 after 500 ms and bar 5 after 1000 ms
 *
 * The bar_status array in a keyfram should always have sequential bars turned on. For example:
 * [1, 1, 1, 1, 0, 0, 0....0] is valid
 * [1, 0, 1, 1, 0, 1, 0....0] is not valid
 *
 * The first keyframe should have all bars in the position you want your anmiation to start and the duration should be
 * 0.
 *
 * @date 2021-08-01
 *
 * @bug No known bugs.
 */

#ifndef ANIMATE_H
#define ANIMATE_H

#include "solar_panel.hpp"
#include <Arduino.h>

// Create a struct to define a keyframe. This keyframe should contain the following:
//     - An array of booleans representing the status of each bar
//     - The duration of the keyframe in milliseconds
//     - A pointer to the next keyframe

// NOTE: As an implentation decision, we could either have the user pass the parameters for the keyframe to the
// animation object or have the user create the keyframe and pass it to the animation object. Because keyframes are
// very simple, I think it makes more sense to have the user pass the parameters to the animation object. If
// animations become more complex, we can change this.
struct Keyframe {
    bool         bar_status[_SOLAR_PANEL_NUM_BARS];
    bool         show_sun;
    unsigned int duration_ms;
    Keyframe    *next;
};

// Create a class to define an animation. The animation should do the following:
//    - Construct as an empty animation
//    - Have a method to add a keyframe to the animation
//        - This method accepts an array of bar statuses and a duration and creates a keyframe with those values
//        - This method should add the keyframe to the end of the animation
//        - This method returns NULL
//    - Have a method to start the animation
//    - Have a method to stop the animation
//    - Have a method to update the animation. This method should do the following:
//        - Check if the animation is running
//        - Using time, determine which keyframe should be active
//        - Set the status of the bars to the status of the active keyframe
//        - If the animation is not running, do nothing
//   - Have a method to get the current keyframe. This method should do the following:
//        - Check if the animation is running
//        - Using time, determine which keyframe should be active
//        - Return the active keyframe
//        - If the animation is not running, return NULL
//   - Have a method to get the status of the animation. This method should do the following:
//        - Return whether or not the animation is running
//   - Have a method to update any keyframe in the animation. This method should do the following:
//        - Accept an index, an array of bar statuses, and a duration
//        - Update the keyframe at the given index with the given values
//        - Return NULL

class AnimateSolarPanel {
  public:
    AnimateSolarPanel();
    void addKeyframe(int num_bars_on, unsigned int duration_ms, bool show_sun = true);
    void addKeyframeFromArray(bool bar_status[_SOLAR_PANEL_NUM_BARS], unsigned int duration_ms, bool show_sun = true);
    void addPauseKeyframe(unsigned int duration_ms);
    void start();
    void stop();
    void update();
    Keyframe *getCurrentKeyframe();
    void      setSolarPanel(SolarPanel *solar_panel);
    bool      isRunning();
    void      updateKeyframe(unsigned int index, bool bar_status[_SOLAR_PANEL_NUM_BARS], unsigned int duration_ms);

  private:
    Keyframe     *first_keyframe;
    Keyframe     *current_keyframe;
    Keyframe     *last_keyframe; // Keyframe we were on last time update() was called. Initialzes to first_keyframe
    SolarPanel   *solar_panel;
    unsigned int  num_bars_on;
    bool          show_sun;
    bool          running;
    unsigned long last_update_time_ms;
    unsigned long time_between_bar_updates_ms;
    int           num_bars_to_update; // Number of bars that need to be updated
    bool          incrementing;

    void _appendKeyframe(Keyframe *keyframe);
    int  _getNumberOfBarsToUpdate(bool last_bar_status[_SOLAR_PANEL_NUM_BARS],
                                  bool current_bar_status[_SOLAR_PANEL_NUM_BARS]);
    void _updateBars();
};

#endif