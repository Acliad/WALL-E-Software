/**
 * @file navigation_controller.hpp
 * @author Isaac Rex (@Acliad)
 * @brief A small wrapper class to help with the controller state. Makes it easy to determine which buttons are pressed
 * or were pressed.
 * TODO: Add the left thumbstick click button
 * @version 0.1
 * @date 2023-10-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef NAVIGATION_CONTROLLER_HPP
#define NAVIGATION_CONTROLLER_HPP

#include <Arduino.h>
#include <Bluepad32.h>

// Controller limits
#define CONTROLLER_THUMBSTICK_MAX (512)
#define CONTROLLER_THUMBSTICK_MIN (-511)
#define CONTROLLER_TRIGGER_MAX    (255)
#define CONTROLLER_TRIGGER_MIN    (0)

// Controller button masks
#define _CONTROLLER_BUTTON_MASK_X          0x01
#define _CONTROLLER_BUTTON_MASK_CIRCLE     0x02
#define _CONTROLLER_BUTTON_MASK_L1         0x10
#define _CONTROLLER_BUTTON_MASK_L2         0x40
#define _CONTROLLER_BUTTON_MASK_THUMBSTICK 0x100
#define _CONTROLLER_MISC_MASK_PS           0x01
#define _CONTROLLER_DPAD_MASK_UP           0x01
#define _CONTROLLER_DPAD_MASK_DOWN         0x02
#define _CONTROLLER_DPAD_MASK_RIGHT        0x04
#define _CONTROLLER_DPAD_MASK_LEFT         0x08

// Class for handling the controller state.
class NavigationController {
  public:
    NavigationController();
    // NOTE: IsPressed() functions return true if the button is currently pressed. WasPressed() functions return true if
    // the button was pressed since the last time the function was called. WasReleased() functions return true if the
    // button was released since the last time the function was called. WasPressed will clear the WasReleased state and
    // vice versa.

    void       setGamepad(GamepadPtr controller);
    GamepadPtr getGamepad();

    void setDeadzone(int deadzone);
    int  getDeadzone(int deadzone);

    bool upIsPressed();
    bool downIsPressed();
    bool leftIsPressed();
    bool rightIsPressed();

    bool upWasPressed();
    bool downWasPressed();
    bool leftWasPressed();
    bool rightWasPressed();

    bool upWasReleased();
    bool downWasReleased();
    bool leftWasReleased();
    bool rightWasReleased();

    // Thumbstick
    bool thumbstickIsPressed();
    bool thumbstickWasPressed();
    bool thumbstickWasReleased();

    // X button
    bool xIsPressed();
    bool xWasPressed();
    bool xWasReleased();

    // circle
    bool circleIsPressed();
    bool circleWasPressed();
    bool circleWasReleased();

    // L1
    bool l1IsPressed();
    bool l1WasPressed();
    bool l1WasReleased();

    // L2
    bool l2IsPressed();
    bool l2WasPressed();
    bool l2WasReleased();
    int  l2Value();

    // PS button
    bool psIsPressed();
    bool psWasPressed();
    bool psWasReleased();

    // Thumbstick
    int   thumbstickX();
    int   thumbstickY();
    float thumbstickXNorm();
    float thumbstickYNorm();

    void update();

  private:
    int _deadzone;

    unsigned int _lastDpadState;
    unsigned int _DpadWasPressed;
    unsigned int _DpadWasReleased;
    unsigned int _lastButtonState;
    unsigned int _buttonWasPressed;
    unsigned int _buttonWasReleased;
    unsigned int _lastMiscButtonState;
    unsigned int _miscButtonWasPressed;
    unsigned int _miscButtonWasReleased;

    GamepadPtr _controller;

    int _mapToDeadzone(int value);
};

#endif // NAVIGATION_CONTROLLER_HPP