#ifndef ANIMATE_SERVO_RECORDER_HPP
#define ANIMATE_SERVO_RECORDER_HPP

#include <cmath>
#include <Arduino.h>
#include "animate_servo.hpp"
#include "servo_keyframe.hpp"
#include "servo_context.hpp"
#include "../display/display.hpp"
#include "servo_player.hpp"

class ServoAnimationRecorder {
  public:
    enum class Inputs { 
        CANCEL, 
        DONE, 
        NEXT, 
        PREV, 
        UP, 
        DOWN, 
        LEFT, 
        RIGHT 
        };
    enum class States { 
        ENTRY, 
        RECORDING, 
        SAVE, 
        CANCEL,
        DONE
        };

    ServoAnimationRecorder(Display& display, ServoContext& servo_context);
    ~ServoAnimationRecorder();

    // Public member functions
    States inputEvent(Inputs input);
    States getState();

    std::unique_ptr<ServoAnimation> takeAnimation();

    void addFunctionToKeyframe(std::function<void()> function);

  private:
    const unsigned int _DEFAULT_KEYFRAME_LENGTH_MS = 1500;
    const unsigned int _KEYFRAME_CHANGE_DURATION_MS = 100; // Time to use for servos when cycling through keyframes
    const int          _MAX_KEYFRAME_LENGTH_MS = 999 * 1000;
    const int          _MIN_KEYFRAME_LENGTH_MS = 0;
    const unsigned int _NUM_CURSOR_POSITIONS = std::log10(_MAX_KEYFRAME_LENGTH_MS) + 1;
    const unsigned int _DEFAULT_CURSOR_POSITION = 3; // xx█.xxx

    States _state;
    Display& _display;
    Display::Mode _display_start_mode;
    std::unique_ptr<ServoAnimation> _animation;
    std::shared_ptr<ServoAnimation> _cycle_animation;
    ServoKeyframe* _current_keyframe;
    ServoContext& _servos;
    ServoPlayer& _servo_player; // Used to move the servos during keyframe changes

    int _keyframe_num;
    int _current_keyframe_duration_ms;
    unsigned int _cursor_position;

    void _setupRecordingState();
    void _handleRecordingInput(Inputs input);
    void _goToNextKeyframe();
    void _goToPrevKeyframe();
    void _moveServosToCurrentKeyframe();
    void _updateKeyframeDuration(Inputs input);
    void _saveCurrentKeyframeServos();
    void _saveRecording();
};

#endif // ANIMATE_SERVO_RECORDER_HPP
