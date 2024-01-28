#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Arduino.h>

class Button {
public:
    // Constructor
    Button(int pin, unsigned int debounce_time_ms, bool pullup = true);

    // Public member functions
    bool isPressed() const;
    bool wasPressed();
    bool wasReleased();

    unsigned long timeInState();

    void update();

private:
    // Private member variables
    int _pin;
    unsigned int _debounce_time_ms;
    unsigned long _last_update_time_ms;
    unsigned long _last_state_change_time_ms;
    bool _current_state;
    bool _last_debounced_state;
    bool _was_pressed;
    bool _was_released;
    bool _pullup;
    unsigned long _time_in_state_ms;
};

#endif // BUTTON_HPP
