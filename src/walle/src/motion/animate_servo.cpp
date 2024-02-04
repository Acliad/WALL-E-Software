#include "animate_servo.hpp"

ServoAnimation::ServoAnimation()
    : _head(nullptr), _current_keyframe(nullptr), _frame_start_time_ms(0), _playing(false),
      _keyframe_has_started(false) {
}

ServoAnimation::~ServoAnimation() {
    // Iterate through the keyframes and delete them
    ServoKeyframe *current = _head;
    while (current != nullptr) {
        ServoKeyframe *next = current->get_next();
        delete current;
        current = next;
    }
}

void ServoAnimation::add_keyframe(ServoKeyframe *keyframe) {
    // Find the end of the linked list and add the new keyframe
    ServoKeyframe *current = _head;
    if (current == nullptr) {
        _head = keyframe;
    } else {
        while (current->get_next() != nullptr) {
            current = current->get_next();
        }
        current->set_next(keyframe);
    }
}

void ServoAnimation::play() {
    // Start the animation
    _playing = true;
    _current_keyframe = _head;
    _keyframe_has_started = false;
    _frame_start_time_ms = millis();
}

void ServoAnimation::stop() {
    // Stop the animation
    _playing = false;
    _current_keyframe = _head;
}

void ServoAnimation::update() {
    // Check if the animation is running, if not, stop
    if (!_playing) {
        return;
    }

    // Check if we reached the end of the animation, if so, stop
    if (_current_keyframe == nullptr) {
        stop();
        return;
    }

    // Check if the current keyframe has started, if not, start it
    if (!_keyframe_has_started) {
        // Start the keyframe
        _keyframe_has_started = true;

        // Set the start time
        _frame_start_time_ms = millis();

        // Setup all the servos in this keyframe
        _current_keyframe->start_keyframe();
    } // Check if we reached the end of the current keyframe, if so, move to the next keyframe
    else if (millis() - _frame_start_time_ms > _current_keyframe->get_duration_ms()) {
        _current_keyframe = _current_keyframe->get_next();
        _keyframe_has_started = false;
        return;
    }

    // Otherwise we're in the middle of a keyframe, update it
    _current_keyframe->update_keyframe();
}

bool ServoAnimation::isPlaying() {
    return _playing;
}
