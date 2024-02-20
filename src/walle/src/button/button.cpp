/**
 * @file button.cpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the implementation for the Button class. This class is used to abstract button input into a
 * friendly interface and provide debouncing logic.
 * @version 0.1
 * @date 2024-02-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "button.hpp"

Button::Button(int pin, unsigned int debounce_time_ms, bool pullup)
    : _pin(pin), _debounce_time_ms(debounce_time_ms), _current_state(false), _last_debounced_state(false),
      _was_pressed(false), _was_released(false), _pullup(pullup), _time_in_state_ms(0), _last_update_time_ms(0),
      _last_state_change_time_ms(millis()) {

    pinMode(_pin, _pullup ? INPUT_PULLUP : INPUT);
}

bool Button::isPressed() const {
    return _current_state;
}

bool Button::wasPressed() {
    bool result = _was_pressed;
    _was_pressed = false; // Clear the state since we read it
    return result;
}

bool Button::wasReleased() {
    bool result = _was_released;
    _was_released = false; // Clear the state since we read it
    return result;
}

unsigned long Button::timeInState() {
    return _time_in_state_ms;
}

void Button::update() {
    bool new_state = digitalRead(_pin) ^ _pullup; // Convert to logical state where 1 is pressed
    unsigned long time_ms = millis();
    // Debounce the button press
    if (new_state != _current_state) {
        if (time_ms - _last_update_time_ms >= _debounce_time_ms) {
            // Debounce time has ellapsed, so update the state
            _last_state_change_time_ms = time_ms;
            _current_state = new_state;
            if (new_state) {
                _was_pressed = true;
                _was_released = false;
            } else {
                _was_released = true;
                _was_pressed = false;
            }
        }
    } else {
        // Reset the debounce timer
        _last_update_time_ms = time_ms;
    }

    // Update the time in state
    _time_in_state_ms = time_ms - _last_state_change_time_ms;
}
