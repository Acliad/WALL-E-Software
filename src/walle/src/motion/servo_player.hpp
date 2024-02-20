/**
 * @file servo_player.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the declaration of the ServoPlayer class, which is used to play servo animations.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef SERVO_PLAYER_H
#define SERVO_PLAYER_H

#include "animate_servo.hpp"

/**
 * @class ServoPlayer
 * @brief The ServoPlayer class is responsible for controlling servo animations.
 * 
 * The ServoPlayer class provides functionality to play, stop, update, and retrieve information about servo animations.
 * It follows the singleton design pattern to ensure that only one instance of the class can exist.
 * The copy constructor and assignment operator are deleted to prevent unintended copying of the class.
 */
class ServoPlayer {
public:
    // Delete the copy constructor and assignment operator
    ServoPlayer(const ServoPlayer&) = delete;
    ServoPlayer& operator=(const ServoPlayer&) = delete;

    /**
     * @brief Get the instance of the ServoPlayer.
     * @return The instance of the ServoPlayer.
     */
    static ServoPlayer& getInstance();

    /**
     * @brief Play a servo animation.
     * @param animation The servo animation to play.
     */
    void play(ServoAnimation* animation);

    /**
     * @brief Stop the currently playing servo animation.
     */
    void stop();

    /**
     * @brief Check if a servo animation is currently playing.
     * @return True if a servo animation is playing, false otherwise.
     */
    bool isPlaying();

    /**
     * @brief Update the servo player.
     * 
     * This method should be called periodically to update the servo player's state.
     * It is responsible for advancing the animation frames and controlling the servos accordingly.
     */
    void update();

    /**
     * @brief Get the currently playing servo animation.
     * @return The currently playing servo animation, or nullptr if no animation is playing.
     */
    ServoAnimation* getCurrentAnimation();

private:
    /**
     * @brief Private constructor to enforce singleton pattern.
     */
    ServoPlayer();

    ServoAnimation* _current_animation; ///< The currently playing servo animation.
    bool _is_playing; ///< Flag indicating if a servo animation is currently playing.
};

#endif // SERVO_PLAYER_H
