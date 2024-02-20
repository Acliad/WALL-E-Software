/**
 * @file navigation_controller.cpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the implementation for the NavigationController class. This class is used to abstract
 * controller input into a friendly interface. It uses the BP32 library to interface with the controller and provides
 * some additional functionality like deadzone mapping and button press/release tracking.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "navigation_controller.hpp"

NavigationController::NavigationController()
    : _controller(nullptr), _lastDpadState(0), _lastButtonState(0), _DpadWasPressed(0), _DpadWasReleased(0),
      _buttonWasPressed(0), _buttonWasReleased(0) {
}

void NavigationController::setGamepad(GamepadPtr controller) {
    _controller = controller;
}

GamepadPtr NavigationController::getGamepad() {
    return _controller;
}

void NavigationController::setDeadzone(int deadzone) {
    // Sets the deadzone for the thumbstick. The thumbstick's values will be remapped such that the value of the
    // deadzone` becomes 0 and the maximum value remains CONTROLLER_THUMBSTICK_MAX (and MIN as appropriate). 
    _deadzone = deadzone;
}

int NavigationController::getDeadzone() {
    return _deadzone;
}

bool NavigationController::upIsPressed() {
    return _lastDpadState & _CONTROLLER_DPAD_MASK_UP;
}

bool NavigationController::downIsPressed() {
    return _lastDpadState & _CONTROLLER_DPAD_MASK_DOWN;
}

bool NavigationController::leftIsPressed() {
    return _lastDpadState & _CONTROLLER_DPAD_MASK_LEFT;
}

bool NavigationController::rightIsPressed() {
    return _lastDpadState & _CONTROLLER_DPAD_MASK_RIGHT;
}

bool NavigationController::upWasPressed() {
    // check if the button was pressed and clear the WasPressed state
    bool result = _DpadWasPressed & _CONTROLLER_DPAD_MASK_UP;
    _DpadWasPressed = _DpadWasPressed & ~_CONTROLLER_DPAD_MASK_UP;
    return result;
}

bool NavigationController::downWasPressed() {
    // check if the button was pressed and clear the WasPressed state
    bool result = _DpadWasPressed & _CONTROLLER_DPAD_MASK_DOWN;
    _DpadWasPressed = _DpadWasPressed & ~_CONTROLLER_DPAD_MASK_DOWN;
    return result;
}

bool NavigationController::leftWasPressed() {
    // check if the button was pressed and clear the WasPressed state
    bool result = _DpadWasPressed & _CONTROLLER_DPAD_MASK_LEFT;
    _DpadWasPressed = _DpadWasPressed & ~_CONTROLLER_DPAD_MASK_LEFT;
    return result;
}

bool NavigationController::rightWasPressed() {
    // check if the button was pressed and clear the WasPressed state
    bool result = _DpadWasPressed & _CONTROLLER_DPAD_MASK_RIGHT;
    _DpadWasPressed = _DpadWasPressed & ~_CONTROLLER_DPAD_MASK_RIGHT;
    return result;   
}

bool NavigationController::upWasReleased() {
    // check if the button was released and clear the WasReleased state
    bool result = _DpadWasReleased & _CONTROLLER_DPAD_MASK_UP;
    _DpadWasReleased = _DpadWasReleased & ~_CONTROLLER_DPAD_MASK_UP;
    return result;
}

bool NavigationController::downWasReleased() {
    // check if the button was released and clear the WasReleased state
    bool result = _DpadWasReleased & _CONTROLLER_DPAD_MASK_DOWN;
    _DpadWasReleased = _DpadWasReleased & ~_CONTROLLER_DPAD_MASK_DOWN;
    return result;
}

bool NavigationController::leftWasReleased() {
    // check if the button was released and clear the WasReleased state
    bool result = _DpadWasReleased & _CONTROLLER_DPAD_MASK_LEFT;
    _DpadWasReleased = _DpadWasReleased & ~_CONTROLLER_DPAD_MASK_LEFT;
    return result;
}

bool NavigationController::rightWasReleased() {
    // check if the button was released and clear the WasReleased state
    bool result = _DpadWasReleased & _CONTROLLER_DPAD_MASK_RIGHT;
    _DpadWasReleased = _DpadWasReleased & ~_CONTROLLER_DPAD_MASK_RIGHT;
    return result;
}

bool NavigationController::thumbstickIsPressed() {
    return _lastButtonState & _CONTROLLER_BUTTON_MASK_THUMBSTICK;
}

bool NavigationController::thumbstickWasPressed() {
    bool result = _buttonWasPressed & _CONTROLLER_BUTTON_MASK_THUMBSTICK;
    _buttonWasPressed = _buttonWasPressed & ~_CONTROLLER_BUTTON_MASK_THUMBSTICK;
    return result;
}

bool NavigationController::thumbstickWasReleased() {
    bool result = _buttonWasReleased & _CONTROLLER_BUTTON_MASK_THUMBSTICK;
    _buttonWasReleased = _buttonWasReleased & ~_CONTROLLER_BUTTON_MASK_THUMBSTICK;
    return result;
}

bool NavigationController::xIsPressed() {
    return _lastButtonState & _CONTROLLER_BUTTON_MASK_X;
}

bool NavigationController::xWasPressed() {
    // check if the button was pressed and clear the WasPressed state
    bool result = _buttonWasPressed & _CONTROLLER_BUTTON_MASK_X;
    _buttonWasPressed = _buttonWasPressed & ~_CONTROLLER_BUTTON_MASK_X;
    return result;
}

bool NavigationController::xWasReleased() {
    // check if the button was released and clear the WasReleased state
    bool result = _buttonWasReleased & _CONTROLLER_BUTTON_MASK_X;
    _buttonWasReleased = _buttonWasReleased & ~_CONTROLLER_BUTTON_MASK_X;
    return result;
}

bool NavigationController::circleIsPressed() {
    return _lastButtonState & _CONTROLLER_BUTTON_MASK_CIRCLE;
}

bool NavigationController::circleWasPressed() {
    // check if the button was pressed and clear the WasPressed state
    bool result = _buttonWasPressed & _CONTROLLER_BUTTON_MASK_CIRCLE;
    _buttonWasPressed = _buttonWasPressed & ~_CONTROLLER_BUTTON_MASK_CIRCLE;
    return result;
}

bool NavigationController::circleWasReleased() {
    // check if the button was released and clear the WasReleased state
    bool result = _buttonWasReleased & _CONTROLLER_BUTTON_MASK_CIRCLE;
    _buttonWasReleased = _buttonWasReleased & ~_CONTROLLER_BUTTON_MASK_CIRCLE;
    return result;
}

// Create skelleton deffinitions for all declerations in the header file.

bool NavigationController::l1IsPressed() {
    return _lastButtonState & _CONTROLLER_BUTTON_MASK_L1;
}

bool NavigationController::l1WasPressed() {
    // check if the button was pressed and clear the WasPressed state
    bool result = _buttonWasPressed & _CONTROLLER_BUTTON_MASK_L1;
    _buttonWasPressed = _buttonWasPressed & ~_CONTROLLER_BUTTON_MASK_L1;
    return result;
}

bool NavigationController::l1WasReleased() {
    // check if the button was released and clear the WasReleased state
    bool result = _buttonWasReleased & _CONTROLLER_BUTTON_MASK_L1;
    _buttonWasReleased = _buttonWasReleased & ~_CONTROLLER_BUTTON_MASK_L1;
    return result;
}

bool NavigationController::l2IsPressed() {
    return _lastButtonState & _CONTROLLER_BUTTON_MASK_L2;
}

bool NavigationController::l2WasPressed() {
    // check if the button was pressed and clear the WasPressed state
    bool result = _buttonWasPressed & _CONTROLLER_BUTTON_MASK_L2;
    _buttonWasPressed = _buttonWasPressed & ~_CONTROLLER_BUTTON_MASK_L2;
    return result;
}

bool NavigationController::l2WasReleased() {
    // check if the button was released and clear the WasReleased state
    bool result = _buttonWasReleased & _CONTROLLER_BUTTON_MASK_L2;
    _buttonWasReleased = _buttonWasReleased & ~_CONTROLLER_BUTTON_MASK_L2;
    return result;
}

int NavigationController::l2Value() {
    if (_controller == nullptr) {
        return 0;
    } else {
        return _controller->brake();
    }
}

bool NavigationController::psIsPressed() {
    return _lastMiscButtonState & _CONTROLLER_MISC_MASK_PS;
}

bool NavigationController::psWasPressed() {
    // check if the button was pressed and clear the WasPressed state
    bool result = _miscButtonWasPressed & _CONTROLLER_MISC_MASK_PS;
    _miscButtonWasPressed = _miscButtonWasPressed & ~_CONTROLLER_MISC_MASK_PS;
    return result;
}

bool NavigationController::psWasReleased() {
    // check if the button was released and clear the WasReleased state
    bool result = _miscButtonWasReleased & _CONTROLLER_MISC_MASK_PS;
    _miscButtonWasReleased = _miscButtonWasReleased & ~_CONTROLLER_MISC_MASK_PS;
    return result;
}

int NavigationController::thumbstickX() {
    if (_controller == nullptr) {
        return 0;
    } else {
        return _mapToDeadzone(_controller->axisX());
    }
}

int NavigationController::thumbstickY() {
    if (_controller == nullptr) {
        return 0;
    } else {
        return _mapToDeadzone(_controller->axisY());
    }
}

float NavigationController::thumbstickXNorm() {
    return (float) this->thumbstickX() / (float) CONTROLLER_THUMBSTICK_MAX;
}

float NavigationController::thumbstickYNorm() {
    return (float) this->thumbstickY() / (float) CONTROLLER_THUMBSTICK_MAX;
}

void NavigationController::update() {
    // Updates the current state of all the buttons. First calls BP32.update(), then checks if any buttons have changed.

    // Update the button states
    BP32.update();

    if (_controller == nullptr) {
        // No controller set, so we can't update the states
        return;
    }

    // Get dpad and button states
    unsigned int dpadState = _controller->dpad();
    unsigned int buttonState = _controller->buttons();
    unsigned int miscButtonState = _controller->miscButtons();

    // Check if any buttons have changed by XORing the current state with the last state
    unsigned int dpadChanged = dpadState ^ _lastDpadState;
    unsigned int buttonChanged = buttonState ^ _lastButtonState;
    unsigned int miscButtonChanged = miscButtonState ^ _lastMiscButtonState;

    // Need to clear any bits in the WasPressed and WasReleased states that have changed since we don't know if they
    // were cleared by a call to WasPressed() or WasReleased(). This just ensures that the WasPressed and WasReleased
    // states are cleared if the buttton was pressed and released without being checked.
    _DpadWasPressed = _DpadWasPressed & ~dpadChanged;
    _DpadWasReleased = _DpadWasReleased & ~dpadChanged;

    _buttonWasPressed = _buttonWasPressed & ~buttonChanged;
    _buttonWasReleased = _buttonWasReleased & ~buttonChanged;
    
    _miscButtonWasPressed = _miscButtonWasPressed & ~miscButtonChanged;
    _miscButtonWasReleased = _miscButtonWasReleased & ~miscButtonChanged;

    // Set the WasPressed states by ANDing the changed states with the current states. This captures new button presses,
    // but we need to OR the result with the WasPressed state to preserve the previous state. The WasPressed state
    // should only be cleared by a called to WasPressed() or if the button is released.
    _DpadWasPressed = dpadChanged & dpadState | _DpadWasPressed;
    _buttonWasPressed = buttonChanged & buttonState | _buttonWasPressed;
    _miscButtonWasPressed = miscButtonChanged & miscButtonState | _miscButtonWasPressed;

    // Set the WasReleased states by ANDing the changed states with the last states. 
    _DpadWasReleased = dpadChanged & _lastDpadState | _DpadWasReleased;
    _buttonWasReleased = buttonChanged & _lastButtonState | _buttonWasReleased;
    _miscButtonWasReleased = miscButtonChanged & _lastMiscButtonState | _miscButtonWasReleased;

    // Update the last states
    _lastDpadState = dpadState;
    _lastButtonState = buttonState;
    _lastMiscButtonState = miscButtonState;
}

int NavigationController::_mapToDeadzone(int value) {
    // Remap the controller input value so that the returned value is 0 at and below the deadzone then linearly scales
    // to the maximum value.
    if (value > _deadzone) {
        return map(value, _deadzone, CONTROLLER_THUMBSTICK_MAX, 0, CONTROLLER_THUMBSTICK_MAX);
    } else if (value < -_deadzone) {
        return map(value, -_deadzone, CONTROLLER_THUMBSTICK_MIN, 0, CONTROLLER_THUMBSTICK_MIN);
    } else {
        return 0;
    }
}
