/**
 * @file servo_player.cpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the implementation of the ServoPlayer class, which is used to play servo animations.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "servo_player.hpp"

ServoPlayer::ServoPlayer() : _current_animation(nullptr), _is_playing(false) {
}

ServoPlayer& ServoPlayer::getInstance() {
    static ServoPlayer instance;  // Guaranteed to be destroyed.
                                  // Instantiated on first use.
    return instance;
}

void ServoPlayer::play(ServoAnimation *animation) {
    stop();
    // Play the animation
    _current_animation = animation;
    if (_current_animation != nullptr) {
        _current_animation->play();
        _is_playing = true;
    }
}

void ServoPlayer::stop() {
    // Stop the current animation
    if (_current_animation != nullptr) {
        _current_animation->stop();
    }
    _current_animation = nullptr;
    _is_playing = false;
}

bool ServoPlayer::isPlaying() {
    return _is_playing;
}

void ServoPlayer::update() {
    // Update the current animation
    if (_is_playing && _current_animation != nullptr) {
        _current_animation->update();
        if (!_current_animation->isPlaying()) {
            stop();
        }
    }
}

ServoAnimation *ServoPlayer::getCurrentAnimation() {
    return _current_animation;
}
