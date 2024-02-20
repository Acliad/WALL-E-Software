/**
 * @file button.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the declaration for the Button class. This class is used to abstract button input into a
 * friendly interface and provide debouncing logic.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Arduino.h>

/**
 * @brief Represents a button with debounce functionality.
 */
class Button {
public:
    /**
     * @brief Constructor for the Button class.
     * 
     * @param pin The pin number to which the button is connected.
     * @param debounce_time_ms The debounce time in milliseconds.
     * @param pullup Flag indicating whether the button is connected with a pull-up resistor (default: true).
     */
    Button(int pin, unsigned int debounce_time_ms, bool pullup = true);

    /**
     * @brief Checks if the button is currently pressed.
     * 
     * @return True if the button is pressed, false otherwise.
     */
    bool isPressed() const;

    /**
     * @brief Checks if the button was pressed since the last update. This state is cleared after being called, so 
     * subsequent calls will return false until the button is pressed again.
     * 
     * @return True if the button was pressed, false otherwise.
     */
    bool wasPressed();

    /**
     * @brief Checks if the button was released since the last update. This state is cleared after being called, so 
     * subsequent calls will return false until the button is pressed again.
     * 
     * @return True if the button was released, false otherwise.
     */
    bool wasReleased();

    /**
     * @brief Returns the time in milliseconds that the button has been in its current state.
     * 
     * @return The time in milliseconds.
     */
    unsigned long timeInState();

    /**
     * @brief Updates the button state.
     * 
     * This function should be called periodically to update the button state.
     */
    void update();

private:
    // Private member variables
    int _pin;                           ///< The pin number to which the button is connected.
    unsigned int _debounce_time_ms;     ///< The debounce time in milliseconds.
    unsigned long _last_update_time_ms; ///< The timestamp of the last update.
    unsigned long _last_state_change_time_ms; ///< The timestamp of the last state change.
    bool _current_state;                ///< The current debounced state of the button.
    bool _last_debounced_state;         ///< The previous debounced state of the button.
    bool _was_pressed;                  ///< Flag indicating if the button was pressed since the last update.
    bool _was_released;                 ///< Flag indicating if the button was released since the last update.
    bool _pullup;                       ///< Flag indicating if the button is connected with a pull-up resistor.
    unsigned long _time_in_state_ms;    ///< The time in milliseconds that the button has been in its current state.
};

#endif // BUTTON_HPP
