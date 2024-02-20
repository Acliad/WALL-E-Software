/**
 * @file animate_servo_recorder.cpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the implementation of the ServoAnimationRecorder class, which is used to record servo
 * animations.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "animate_servo_recorder.hpp"

ServoAnimationRecorder::ServoAnimationRecorder(Display &display, ServoContext &servo_context)
    : _state(States::ENTRY), _animation(new ServoAnimation()), _display(display),
      _display_start_mode(display.getMode()), _keyframe_num(0), _servos(servo_context),
      _current_keyframe(new ServoKeyframe(_DEFAULT_KEYFRAME_LENGTH_MS)), _cursor_position(_DEFAULT_CURSOR_POSITION),
      _servo_player(ServoPlayer::getInstance()), _cycle_animation(nullptr) {

    _display.setMode(Display::Mode::RECORDER);
    _display.recording_panel.setStartPage();

    // Add the initial (head) keyframe to this animation
    _animation->set_head(_current_keyframe);
}

ServoAnimationRecorder::~ServoAnimationRecorder() {
    _display.setMode(_display_start_mode);

    // Delete the animation (and bound keyframes) if the animation was not taken
    if (_animation != nullptr) {
        delete _animation;
    }

    // Make sure the servo player isn't trying to play the cycle animation
    if (_servo_player.getCurrentAnimation() == _cycle_animation) {
        _servo_player.stop();
    }
    // Delete the cycle animation if it exists
    if (_cycle_animation != nullptr) {
        delete _cycle_animation;
    }
}

ServoAnimationRecorder::States ServoAnimationRecorder::inputEvent(Inputs input) {
    switch (_state) {
    case States::ENTRY:
        if (input == Inputs::UP || input == Inputs::DOWN || input == Inputs::LEFT || input == Inputs::RIGHT) {
            _updateRecordingStateDisplay();
            _state = States::RECORDING;
        } else if (input == Inputs::CANCEL) {
            _display.recording_panel.setCancelPage();
            _state = States::CANCEL;
        }
        break;
    case States::RECORDING:
        if (input == Inputs::DONE) {
            _display.recording_panel.setSavePage();
            _state = States::SAVE;
        } else if (input == Inputs::CANCEL) {
            _display.recording_panel.setCancelPage();
            _state = States::CANCEL;
        } else {
            _handleRecordingInput(input);
        }
        break;
    case States::SAVE:
        if (input == Inputs::DONE) {
            _saveCurrentKeyframeServos();
            _display.setMode(_display_start_mode);
            _state = States::DONE;
        } else {
            _updateRecordingStateDisplay();
            _state = States::RECORDING;
        }
        break;
    case States::CANCEL:
        if (input == Inputs::CANCEL) {
            delete _animation;
            _animation = nullptr;
            _display.setMode(_display_start_mode);
            _state = States::DONE;
        }
        else {
            _updateRecordingStateDisplay();
            _state = States::RECORDING;
        }
        break;
    }
    return _state;
}

ServoAnimationRecorder::States ServoAnimationRecorder::getState() {
    return _state;
}

ServoAnimation *ServoAnimationRecorder::takeAnimation() {
    if (_state != States::DONE) {
        return nullptr;
    }
    ServoAnimation *ret = _animation;
    _animation = nullptr;
    _current_keyframe = nullptr;
    return ret;
}

void ServoAnimationRecorder::setAnimation(ServoAnimation *animation) {
    if (_animation != nullptr) {
        delete _animation;
    }
    _animation = new ServoAnimation(*animation);
    _current_keyframe = _animation->get_head();
    _keyframe_num = 0;
    _cursor_position = _DEFAULT_CURSOR_POSITION;
    _moveServosToCurrentKeyframe();
}

void ServoAnimationRecorder::addTrackToKeyframe(int track_index, DfMp3 *dfmp3) {
    if (_current_keyframe != nullptr) {
        _current_keyframe->add_track(track_index, dfmp3);
    }
}

void ServoAnimationRecorder::_updateRecordingStateDisplay() {
    _display.recording_panel.setRecordingPage(_current_keyframe->get_duration(), _cursor_position, _keyframe_num,
                                              &_servos);
}

void ServoAnimationRecorder::_handleRecordingInput(Inputs input) {

    switch (input) {
    case Inputs::UP:
    case Inputs::DOWN:
    case Inputs::LEFT:
    case Inputs::RIGHT:
        _updateKeyframeDuration(input);
        break;
    case Inputs::NEXT:
        _goToNextKeyframe();
        break;
    case Inputs::PREV:
        _goToPrevKeyframe();
        break;
    case Inputs::DELETE:
        _deleteCurrentKeyframe();
    default:
        return;
    }

    // Update the display
    _updateRecordingStateDisplay();
}

void ServoAnimationRecorder::_goToNextKeyframe() {
    _saveCurrentKeyframeServos(); // Update the current keyframe's servos in case they were moved
    // If the current keyframe has a next keyframe, move to it. Otherwise, create a new keyframe
    if (_current_keyframe->get_next() != nullptr) {
        _current_keyframe = _current_keyframe->get_next();
        _moveServosToCurrentKeyframe();
    } else {
        // We were already at the tail, so create a new keyframe
        ServoKeyframe *new_keyframe = new ServoKeyframe(_DEFAULT_KEYFRAME_LENGTH_MS);
        _current_keyframe->set_next(new_keyframe);
        new_keyframe->set_prev(_current_keyframe);
        _current_keyframe = new_keyframe;
    }
    _keyframe_num++;
}

void ServoAnimationRecorder::_goToPrevKeyframe() {
    _saveCurrentKeyframeServos(); // Update the current keyframe's servos in case they were moved
    // If the current keyframe has a previous keyframe, move to it
    if (_current_keyframe->get_prev() != nullptr) {
        // Move to the previous keyframe
        _current_keyframe = _current_keyframe->get_prev();
        _keyframe_num--;
        _moveServosToCurrentKeyframe();
    }
}

void ServoAnimationRecorder::_deleteCurrentKeyframe() {
    // Remove the current keyframe from the animtion. Defaults to going to the next keyframe if it exists, otherwise
    // the previous keyframe
    ServoKeyframe *to_delete = _current_keyframe;

    // If there is another keyframe after this one, move to it
    if (_current_keyframe->get_next() != nullptr) {
        _goToNextKeyframe();
        _keyframe_num--; // Next keyframe is now the current keyframe, so decrement the keyframe number
    } else if (_current_keyframe->get_prev() != nullptr) { // If there is another keyframe before this one, move to it
        _goToPrevKeyframe();
    } else { // If there are no other keyframes, we're at the head and we can't delete it
        return;
    }

    // If we removed the head, set the new head
    if (to_delete == _animation->get_head()) {
        _animation->set_head(_current_keyframe);
    }
    // Remove the keyframe from the animation
    delete to_delete; // NOTE: The deconstructor for ServoKeyframe will restitch the list
    _updateRecordingStateDisplay();
}

void ServoAnimationRecorder::_moveServosToCurrentKeyframe() {
    // Moves all the servos to the position for this keyframe. This is used when cycling through keyframes so the 
    // servo position can be edited. servo_player singleton is used to make it easy to syncronize with the rest of the
    // servo control code
    if (_servo_player.isPlaying()) {
        _servo_player.stop();
    }

    // Delete the current cycle animation if it exists
    if (_cycle_animation != nullptr) {
        delete _cycle_animation;
    }
    _cycle_animation = new ServoAnimation();
    ServoKeyframe *isolated_keyframe = new ServoKeyframe(*_current_keyframe);
    isolated_keyframe->set_next(nullptr);
    isolated_keyframe->set_prev(nullptr);
    isolated_keyframe->set_duration(_KEYFRAME_CHANGE_DURATION_MS);
    _cycle_animation->add_keyframe(isolated_keyframe);
    _servo_player.play(_cycle_animation);
}

void ServoAnimationRecorder::_updateKeyframeDuration(Inputs input) {
    int _new_keyframe_duration_ms = _current_keyframe->get_duration();
    switch (input) {
    case Inputs::UP:
        _new_keyframe_duration_ms += std::pow(10, _cursor_position);
        break;
    case Inputs::DOWN:
        _new_keyframe_duration_ms -= std::pow(10, _cursor_position);
        break;
    case Inputs::LEFT:
        _cursor_position = (_cursor_position + 1) % _NUM_CURSOR_POSITIONS;
        break;
    case Inputs::RIGHT:
        // Decrement the cursor position. Since it's unsigned, we need to check if it's 0 before decrementing
        _cursor_position =
            _cursor_position > 0 ? (_cursor_position - 1) % _NUM_CURSOR_POSITIONS : _NUM_CURSOR_POSITIONS - 1;
        break;
    }
    // Constrain the duration to the min and max values
    _new_keyframe_duration_ms =
        constrain(_new_keyframe_duration_ms, _MIN_KEYFRAME_LENGTH_MS, _MAX_KEYFRAME_LENGTH_MS);
    _current_keyframe->set_duration(_new_keyframe_duration_ms);
}

void ServoAnimationRecorder::_saveCurrentKeyframeServos() {
    for (auto &servo : _servos.map) {
        _current_keyframe->add_servo_scalar(servo.second, servo.second->get_current_scalar());
    }
}
