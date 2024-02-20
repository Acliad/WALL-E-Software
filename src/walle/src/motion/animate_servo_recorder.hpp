/**
 * @file animate_servo_recorder.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the declaration of the ServoAnimationRecorder class, which is used to record servo
 * animations.
 * @version 0.1
 * @date 2024-02-19
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef ANIMATE_SERVO_RECORDER_HPP
#define ANIMATE_SERVO_RECORDER_HPP

#include <cmath>
#include <Arduino.h>
#include "animate_servo.hpp"
#include "servo_keyframe.hpp"
#include "servo_context.hpp"
#include "../display/display.hpp"
#include "../audio/audio_player.hpp"
#include "servo_player.hpp"

/**
 * @brief Class for recording servo animations.
 *
 * This class provides functionality for recording servo animations. It allows the user to control the animation
 * recording process, add keyframes, and save the animation.
 */
class ServoAnimationRecorder {
  public:
    /**
     * @brief Enum for input events.
     */
    enum class Inputs { 
        CANCEL,    /**< Cancel input */
        DONE,      /**< Done input */
        NEXT,      /**< Next input */
        PREV,      /**< Previous input */
        DELETE,    /**< Delete input */
        UP,        /**< Up input */
        DOWN,      /**< Down input */
        LEFT,      /**< Left input */
        RIGHT      /**< Right input */
    };

    /**
     * @brief Enum for states of the recorder.
     */
    enum class States { 
        ENTRY,     /**< Entry state */
        RECORDING, /**< Recording state */
        SAVE,      /**< Save state */
        CANCEL,    /**< Cancel state */
        DONE       /**< Done state */
    };

    /**
     * @brief Constructor for ServoAnimationRecorder.
     * 
     * @param display The display object used for UI.
     * @param servo_context The servo context object.
     */
    ServoAnimationRecorder(Display& display, ServoContext& servo_context);

    /**
     * @brief Destructor for ServoAnimationRecorder.
     */
    ~ServoAnimationRecorder();

    /**
     * @brief Handles input events and updates the state of the recorder. This is the main method for controlling the
     * recorder.
     * 
     * @param input The input event.
     * @return The new state of the recorder.
     */
    States inputEvent(Inputs input);

    /**
     * @brief Gets the current state of the recorder.
     * 
     * @return The current state of the recorder.
     */
    States getState();

    /**
     * @brief Takes ownership of the animation object. This method is used to transfer the animation object to the
     * ServoAnimationPlayer. It should only be called once and after the recording is complete. It will return a nullptr
     * if the animation object has already been taken or if the recording is not in the DONE state.
     * 
     * @return The animation object.
     */
    ServoAnimation *takeAnimation();

    /**
     * @brief Sets the animation object. This is used to edit already created animations. This can be passed at any
     * time.
     *
     * @param animation The animation object.
     */
    void setAnimation(ServoAnimation *animation);

    /**
     * @brief Adds a track to the current keyframe.
     * 
     * @param track_index The index of the track.
     * @param _dfmp3 The DfMp3 object.
     */
    void addTrackToKeyframe(int track_index, DfMp3 *_dfmp3);

  private:
    const unsigned int _DEFAULT_KEYFRAME_LENGTH_MS = 1500; /**< Default length of a keyframe in milliseconds */
    const unsigned int _KEYFRAME_CHANGE_DURATION_MS = 100; /**< Duration of servo movement when cycling through keyframes */
    const int _MAX_KEYFRAME_LENGTH_MS = 999 * 1000;       /**< Maximum length of a keyframe in milliseconds */
    const int _MIN_KEYFRAME_LENGTH_MS = 0;                /**< Minimum length of a keyframe in milliseconds */
    const unsigned int _NUM_CURSOR_POSITIONS =
        std::log10(_MAX_KEYFRAME_LENGTH_MS) + 1;          /**< Number of cursor positions for keyframe length */
    const unsigned int _DEFAULT_CURSOR_POSITION = 3;      /**< Default cursor position for keyframe length */

    States _state;                       /**< Current state of the recorder */
    Display& _display;                   /**< Display object used for UI */
    Display::Mode _display_start_mode;   /**< Start mode of the display tracked so it can be reset on completion */
    ServoAnimation *_animation;          /**< Animation object */
    ServoAnimation *_cycle_animation;    /**< Animation object used for cycling through keyframes */
    ServoKeyframe* _current_keyframe;    /**< Current keyframe */
    ServoContext& _servos;               /**< Servo context object */
    ServoPlayer& _servo_player;          /**< Servo player object used for moving servos during keyframe changes */

    int _keyframe_num;                   /**< Number of keyframes */
    int _current_keyframe_duration_ms;   /**< Duration of the current keyframe in milliseconds */
    unsigned int _cursor_position;       /**< Current cursor position for keyframe length */

    /**
     * @brief Updates the display during the recording state.
     */
    void _updateRecordingStateDisplay();

    /**
     * @brief Handles input events during the recording state.
     * 
     * @param input The input event.
     */
    void _handleRecordingInput(Inputs input);

    /**
     * @brief Moves to the next keyframe.
     */
    void _goToNextKeyframe();

    /**
     * @brief Moves to the previous keyframe.
     */
    void _goToPrevKeyframe();

    /**
     * @brief Deletes the current keyframe.
     */
    void _deleteCurrentKeyframe();

    /**
     * @brief Moves the servos to the current keyframe.
     */
    void _moveServosToCurrentKeyframe();

    /**
     * @brief Updates the duration of the current keyframe based on the input event.
     * 
     * @param input The input event.
     */
    void _updateKeyframeDuration(Inputs input);

    /**
     * @brief Saves the servo states of all the servos to the current keyframe.
     */
    void _saveCurrentKeyframeServos();
};

#endif // ANIMATE_SERVO_RECORDER_HPP
