/**
 * @file animate_servo.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the declaration of the ServoAnimation class, which is used to create and play servo
 * animations.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ANIMATE_SERVO_HPP
#define ANIMATE_SERVO_HPP

#include <FS.h>
#include <SPIFFS.h>
#include <Arduino.h>
#include <Ramp.h>
#include <sstream>
#include <string>
#include <iostream>
#include "servo_keyframe.hpp"
#include "servo_context.hpp"

/**
 * @brief Class representing a servo animation.
 */
class ServoAnimation {
  public:
    /**
     * @brief Default constructor.
     */
    ServoAnimation();

    /**
     * @brief Copy constructor. Iterates through the keyframes of the other animation and adds coppies of them to this
     * one.
     * @param other The ServoAnimation object to copy from.
     */
    ServoAnimation(const ServoAnimation &other);

    /**
     * @brief Destructor.
     */
    ~ServoAnimation();

    /**
     * @brief Adds a keyframe to the animation. The given keyframe does not have to be the first keyframe in a linked
     * list. This function will add all keyframes in the linked list to the animation by searching for the head of the
     * given list and appending it to the tail of the current list.
     * @param keyframe The keyframe to add.
     */
    void add_keyframe(ServoKeyframe *keyframe);

    /**
     * @brief Starts playing the animation.
     */
    void play();

    /**
     * @brief Stops the animation.
     */
    void stop();

    /**
     * @brief Updates the animation. This function should be called periodically to update the animation.
     */
    void update();

    /**
     * @brief Checks if the animation is currently playing.
     * @return True if the animation is playing, false otherwise.
     */
    bool isPlaying();

    /**
     * @brief Gets the head keyframe of the animation.
     * @return The head keyframe.
     */
    ServoKeyframe *get_head();

    /**
     * @brief Sets the head keyframe of the animation. The user is responsible for deleting the old head keyframe if
     * necessary.
     * @param head The head keyframe to set.
     */
    void set_head(ServoKeyframe *head);

    /**
     * @brief Saves the animation to a file.
     * @param filesystem The file system to save to.
     * @param filename The name of the file to save.
     * @return True if the save operation was successful, false otherwise.
     */
    bool save(fs::FS &filesystem, const char* filename);

    /**
     * @brief Loads an animation from a file.
     * @param filesystem The file system to load from.
     * @param filename The name of the file to load.
     * @param servo_context The servo context to use for loading.
     * @param _dfmp3 The DfMp3 object to use for loading.
     * @return A pointer to the loaded ServoAnimation object, or nullptr if loading failed.
     */
    static ServoAnimation* load(fs::FS &filesystem, const char *filename, ServoContext &servo_context,
                                                DfMp3 *_dfmp3);

    /**
     * @brief Prints debug information about the animation.
     */
    void printDebugInfo();

  private:
    ServoKeyframe *_head; /**< The head keyframe of the animation. */
    ServoKeyframe *_current_keyframe; /**< The current keyframe being played. */
    unsigned long _frame_start_time_ms; /**< The start time of the current frame. */
    bool _playing; /**< Flag indicating if the animation is currently playing. */
    bool _keyframe_has_started; /**< Flag indicating if the current keyframe has started. */

    static constexpr char* const _SERIALIZED_KEYFRAME_START = "start keyframe"; /**< Serialized keyframe start mark. */
    static constexpr char* const _SERIALIZED_KEYFRAME_END   = "end keyframe"; /**< Serialized keyframe end mark. */
};

#endif // ANIMATE_SERVO_HPP