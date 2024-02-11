#include "animate_servo_recorder.hpp"

ServoAnimationRecorder::ServoAnimationRecorder(Display &display, ServoContext &servo_context)
    : _state(States::ENTRY), _animation(new ServoAnimation()), _display(display),
      _display_start_mode(display.getMode()), _keyframe_num(0), _servos(servo_context), _current_keyframe(nullptr),
      _cursor_position(_DEFAULT_CURSOR_POSITION), _servo_player(ServoPlayer::getInstance()), _cycle_animation(nullptr) {
    _display.setMode(Display::Mode::RECORDER);
    _display.recording_panel.setStartPage();
}

ServoAnimationRecorder::~ServoAnimationRecorder() {
    _display.setMode(_display_start_mode);

    // Delete the keyframes if the animation was not taken
    if (_animation != nullptr && _current_keyframe != nullptr) {
        // Delete any keyframes ahead of the current keyframe
        ServoKeyframe *next = _current_keyframe->get_next();
        while (next != nullptr) {
            ServoKeyframe *temp = next;
            next = next->get_next();
            delete temp;
        }
        // Delete any keyframes behind the current keyframe
        ServoKeyframe *prev = _current_keyframe->get_prev();
        while (prev != nullptr) {
            ServoKeyframe *temp = prev;
            prev = prev->get_prev();
            delete temp;
        }
        // Delete the current keyframe
        delete _current_keyframe;
        _current_keyframe = nullptr;
    }
}

ServoAnimationRecorder::States ServoAnimationRecorder::inputEvent(Inputs input) {
    switch (_state) {
    case States::ENTRY:
        if (input == Inputs::UP || input == Inputs::DOWN || input == Inputs::LEFT || input == Inputs::RIGHT) {
            _setupRecordingState();
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
            // NOTE: add_keyframe will find the first keyframe in the list and add the new keyframe list to the end of
            // the animation, so we don't need to worry about the current keyframe's position in the list
            _animation->add_keyframe(_current_keyframe);
            _display.setMode(_display_start_mode);
            _state = States::DONE;
        } else {
            _display.recording_panel.setRecordingPage(_current_keyframe->get_duration(), _cursor_position,
                                                      _keyframe_num, &_servos);
            _state = States::RECORDING;
        }
        break;
    case States::CANCEL:
        if (input == Inputs::CANCEL) {
            _display.setMode(_display_start_mode);
            _animation = nullptr;
            _state = States::DONE;
        }
        else {
            _display.recording_panel.setRecordingPage(_current_keyframe->get_duration(), _cursor_position,
                                                      _keyframe_num, &_servos);
            _state = States::RECORDING;
        }
        break;
    }
    return _state;
}

ServoAnimationRecorder::States ServoAnimationRecorder::getState() {
    return _state;
}

std::unique_ptr<ServoAnimation> ServoAnimationRecorder::takeAnimation() {
    return std::move(_animation);
}

void ServoAnimationRecorder::save(ServoAnimation &animation, const char *filename) {
    // TODO: Implement
}

void ServoAnimationRecorder::load(ServoAnimation &animation, const char *filename) {
    // TODO: Implement
}

void ServoAnimationRecorder::_setupRecordingState() {
    _current_keyframe = new ServoKeyframe(_DEFAULT_KEYFRAME_LENGTH_MS);
    _display.recording_panel.setRecordingPage(_DEFAULT_KEYFRAME_LENGTH_MS, _cursor_position, _keyframe_num, &_servos);
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
    default:
        return;
    }

    // Update the display
    _display.recording_panel.setRecordingPage(_current_keyframe->get_duration(), _cursor_position, _keyframe_num,
                                              &_servos);
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

void ServoAnimationRecorder::_moveServosToCurrentKeyframe() {
    // Moves all the servos to the position for this keyframe. This is used when cycling through keyframes so the 
    // servo position can be edited. servo_player singleton is used to make it easy to syncronize with the rest of the
    // servo control code
    if (_servo_player.isPlaying()) {
        _servo_player.stop();
    }
    _cycle_animation = std::make_shared<ServoAnimation>();
    ServoKeyframe *isolated_keyframe = new ServoKeyframe(*_current_keyframe);
    isolated_keyframe->set_next(nullptr);
    isolated_keyframe->set_prev(nullptr);
    isolated_keyframe->set_duration(_KEYFRAME_CHANGE_DURATION_MS);
    _cycle_animation->add_keyframe(isolated_keyframe);
    _servo_player.play(_cycle_animation.get());
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
    // Create a lambda function that takes a ServoMotor as an argument and adds the current position to the keyframe
    auto add_servo_position = [this](ServoMotor *servo) {
        _current_keyframe->add_servo_scalar(servo, servo->get_current_scalar());
    };
    // Apply the lambda to all the servos
    _servos.applyToAllServos(add_servo_position);
}

// TODO: Remove?
void ServoAnimationRecorder::_saveRecording() {
    // Latest keyframe won't have been saved yet
    // Add the keyframes to the animation. add_keyframe will find the first keyframe in the list and add the new
    // keyframe list to the end of the animation
    // _animation.add_keyframe(_current_keyframe);
}
