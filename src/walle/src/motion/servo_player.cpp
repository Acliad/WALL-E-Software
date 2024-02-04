#include "servo_player.hpp"

ServoPlayer::ServoPlayer() : _current_animation(nullptr) {
    // Initialize the ServoPlayer here
}

ServoPlayer& ServoPlayer::getInstance() {
    static ServoPlayer instance;  // Guaranteed to be destroyed.
                                  // Instantiated on first use.
    return instance;
}

void ServoPlayer::play(ServoAnimation *animation) {
    // Play the animation
    _current_animation = animation;
    _current_animation->play();
}

void ServoPlayer::stop() {
    // Stop the current animation
    if (_current_animation != nullptr) {
        _current_animation->stop();
    }
}

bool ServoPlayer::isPlaying() {
    if (_current_animation != nullptr) {
        return _current_animation->isPlaying();
    }
    return false;
}

void ServoPlayer::update() {
    // Update the current animation
    if (_current_animation != nullptr) {
        _current_animation->update();
    }
}
