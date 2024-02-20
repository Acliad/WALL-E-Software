/**
 * @file animate_solar_panel.hpp
 * @brief This file defines the animation class for the solar panel. This class is used to create animations for the
 * solar elements by defining keyframes and the duration of each keyframe. The class will then update the solar panel
 * to match the keyframe at the current time. The class also has the ability to play audio tracks at specific keyframes.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 */

#ifndef ANIMATE_H
#define ANIMATE_H

#include "solar_panel.hpp"
#include "../audio/audio_player.hpp"
#include <Arduino.h>

// NOTE: As an implentation decision, we could either have the user pass the parameters for the keyframe to the
// animation object or have the user create the keyframe and pass it to the animation object. Because keyframes are
// very simple, I think it makes more sense to have the user pass the parameters to the animation object. If
// animations become more complex, we can change this.
struct Keyframe {
    bool         bar_status[_SOLAR_PANEL_NUM_BARS];
    bool         show_sun;
    unsigned int duration_ms;
    unsigned int track_index;
    DfMp3       *dfmp3;
    Keyframe    *next;
};

/**
 * @class AnimateSolarPanel
 * @brief Class for animating a solar panel display.
 *
 * This class provides methods to add keyframes, start and stop the animation, and update the display.
 * It also allows for setting the solar panel object and checking the current state of the animation.
 */
class AnimateSolarPanel {
  public:
    /**
     * @brief Default constructor for AnimateSolarPanel.
     */
    AnimateSolarPanel();

    /**
     * @brief Add a keyframe with the specified number of bars turned on and duration.
     * @param num_bars_on The number of bars that should be on at the end of the keyframe.
     * @param duration_ms The duration of the keyframe in milliseconds.
     * @param show_sun Flag indicating whether to show the sun in the keyframe (default: true).
     */
    void addKeyframe(int num_bars_on, unsigned int duration_ms, bool show_sun = true);

    /**
     * @brief Add a keyframe with the specified number of bars turned on, duration, and audio track.
     * @param num_bars_on The number of bars that should be on at the end of the keyframe.
     * @param duration_ms The duration of the keyframe in milliseconds.
     * @param track_index The index of the audio track to play during the keyframe.
     * @param dfmp3 A pointer to the DfMp3 object for audio playback.
     * @param show_sun Flag indicating whether to show the sun in the keyframe (default: true).
     */
    void addKeyframe(int num_bars_on, unsigned int duration_ms, unsigned int track_index, DfMp3 *dfmp3,
                     bool show_sun = true);

    /**
     * @brief Add a keyframe with the specified bar status array and duration.
     * @param bar_status An array indicating the status of each bar in the keyframe.
     * @param duration_ms The duration of the keyframe in milliseconds.
     * @param show_sun Flag indicating whether to show the sun in the keyframe (default: true).
     */
    void addKeyframeFromArray(bool bar_status[_SOLAR_PANEL_NUM_BARS], unsigned int duration_ms, bool show_sun = true);

    /**
     * @brief Add a keyframe with the specified bar status array, duration, and audio track.
     * @param bar_status An array indicating the status of each bar in the keyframe.
     * @param duration_ms The duration of the keyframe in milliseconds.
     * @param track_index The index of the audio track to play during the keyframe.
     * @param dfmp3 A pointer to the DfMp3 object for audio playback.
     * @param show_sun Flag indicating whether to show the sun in the keyframe (default: true).
     */
    void addKeyframeFromArray(bool bar_status[_SOLAR_PANEL_NUM_BARS], unsigned int duration_ms,
                              unsigned int track_index, DfMp3 *dfmp3, bool show_sun = true);

    /**
     * @brief Add a pause keyframe with the specified duration.
     * @param duration_ms The duration of the pause keyframe in milliseconds.
     */
    void addPauseKeyframe(unsigned int duration_ms);

    /**
     * @brief Add a pause keyframe with the specified duration and audio track.
     * @param duration_ms The duration of the pause keyframe in milliseconds.
     * @param track_index The index of the audio track to play during the pause keyframe.
     * @param dfmp3 A pointer to the DfMp3 object for audio playback.
     */
    void addPauseKeyframe(unsigned int duration_ms, unsigned int track_index, DfMp3 *dfmp3);

    /**
     * @brief Start the animation.
     */
    void start();

    /**
     * @brief Stop the animation.
     */
    void stop();

    /**
     * @brief Update the animation. Should be called periodically to update the display.
     */
    void update();

    /**
     * @brief Get the current keyframe.
     * @return A pointer to the current keyframe.
     */
    Keyframe *getCurrentKeyframe();

    /**
     * @brief Set the solar panel object.
     * @param solar_panel A pointer to the SolarPanel object.
     */
    void setSolarPanel(SolarPanel *solar_panel);

    /**
     * @brief Check if the animation is running.
     * @return True if the animation is running, false otherwise.
     */
    bool isRunning();

    /**
     * @brief Update a keyframe at the specified index with the given bar status and duration.
     * @param index The index of the keyframe to update.
     * @param bar_status An array indicating the status of each bar in the keyframe.
     * @param duration_ms The duration of the keyframe in milliseconds.
     */
    void updateKeyframe(unsigned int index, bool bar_status[_SOLAR_PANEL_NUM_BARS], unsigned int duration_ms);

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

    /**
     * @brief Append a keyframe to the animation.
     * @param keyframe A pointer to the keyframe to append.
     */
    void _appendKeyframe(Keyframe *keyframe);

    /**
     * @brief Get the number of bars to update between two keyframes.
     * @param last_bar_status An array indicating the status of each bar in the previous keyframe.
     * @param current_bar_status An array indicating the end status of each bar in the current keyframe.
     * @return The number of bars to update.
     */
    int _getNumberOfBarsToUpdate(bool last_bar_status[_SOLAR_PANEL_NUM_BARS],
                                  bool current_bar_status[_SOLAR_PANEL_NUM_BARS]);

    /**
     * @brief Update the bars on the solar panel.
     */
    void _updateBars();
};

#endif