/**
 * @file navigation_controller.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the declaration for the NavigationController class. This class is used to abstract
 * controller input into a friendly interface. It uses the BP32 library to interface with the controller and provides
 * some additional functionality like deadzone mapping and button press/release tracking.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
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

/**
 * @class NavigationController
 * @brief A class that handles the navigation controls of a gamepad.
 */
class NavigationController {
  public:
    /**
     * @brief Default constructor for NavigationController.
     */
    NavigationController();

    /**
     * @brief Sets the gamepad for the NavigationController.
     * @param controller A pointer to the Gamepad object.
     */
    void setGamepad(GamepadPtr controller);

    /**
     * @brief Gets the gamepad associated with the NavigationController.
     * @return A pointer to the Gamepad object.
     */
    GamepadPtr getGamepad();

    /**
     * @brief Sets the deadzone value for the navigation controls. The range of the output values will be remapped such
     * that the raw value at deadzone becomes 0 and the output is linearaly scaled from that point to the max/min value.
     * This preserves the smooth transition of the thumbstick while removing drift. 
     * @param deadzone The deadzone value to set.
     */
    void setDeadzone(int deadzone);

    /**
     * @brief Gets the deadzone value for the navigation controls.
     * @return The deadzone value.
     */
    int getDeadzone();

    /**
     * @brief Checks if the up button is currently pressed.
     * @return True if the up button is pressed, false otherwise.
     */
    bool upIsPressed();

    /**
     * @brief Checks if the down button is currently pressed.
     * @return True if the down button is pressed, false otherwise.
     */
    bool downIsPressed();

    /**
     * @brief Checks if the left button is currently pressed.
     * @return True if the left button is pressed, false otherwise.
     */
    bool leftIsPressed();

    /**
     * @brief Checks if the right button is currently pressed.
     * @return True if the right button is pressed, false otherwise.
     */
    bool rightIsPressed();

    /**
     * @brief Checks if the up button was pressed since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the up button was pressed, false otherwise.
     */
    bool upWasPressed();

    /**
     * @brief Checks if the down button was pressed since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the down button was pressed, false otherwise.
     */
    bool downWasPressed();

    /**
     * @brief Checks if the left button was pressed since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the left button was pressed, false otherwise.
     */
    bool leftWasPressed();

    /**
     * @brief Checks if the right button was pressed since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the right button was pressed, false otherwise.
     */
    bool rightWasPressed();

    /**
     * @brief Checks if the up button was released since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the up button was released, false otherwise.
     */
    bool upWasReleased();

    /**
     * @brief Checks if the down button was released since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the down button was released, false otherwise.
     */
    bool downWasReleased();

    /**
     * @brief Checks if the left button was released since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the left button was released, false otherwise.
     */
    bool leftWasReleased();

    /**
     * @brief Checks if the right button was released since the last time this function was called. This state is
     * cleared after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the right button was released, false otherwise.
     */
    bool rightWasReleased();

    /**
     * @brief Checks if the thumbstick button is currently pressed. This state is cleared after being called, so
     * subsequent calls will return false until the PS button is released again.
     * @return True if the thumbstick button is pressed, false otherwise.
     */
    bool thumbstickIsPressed();

    /**
     * @brief Checks if the thumbstick button was pressed since the last time this function was called. This state is
     * cleared after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the thumbstick button was pressed, false otherwise.
     */
    bool thumbstickWasPressed();

    /**
     * @brief Checks if the thumbstick button was released since the last time this function was called. This state is
     * cleared after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the thumbstick button was released, false otherwise.
     */
    bool thumbstickWasReleased();

    /**
     * @brief Checks if the X button is currently pressed.
     * @return True if the X button is pressed, false otherwise.
     */
    bool xIsPressed();

    /**
     * @brief Checks if the X button was pressed since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the X button was pressed, false otherwise.
     */
    bool xWasPressed();

    /**
     * @brief Checks if the X button was released since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the X button was released, false otherwise.
     */
    bool xWasReleased();

    /**
     * @brief Checks if the circle button is currently pressed.
     * @return True if the circle button is pressed, false otherwise.
     */
    bool circleIsPressed();

    /**
     * @brief Checks if the circle button was pressed since the last time this function was called. This state is
     * cleared after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the circle button was pressed, false otherwise.
     */
    bool circleWasPressed();

    /**
     * @brief Checks if the circle button was released since the last time this function was called. This state is
     * cleared after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the circle button was released, false otherwise.
     */
    bool circleWasReleased();

    /**
     * @brief Checks if the L1 button is currently pressed.
     * @return True if the L1 button is pressed, false otherwise.
     */
    bool l1IsPressed();

    /**
     * @brief Checks if the L1 button was pressed since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the L1 button was pressed, false otherwise.
     */
    bool l1WasPressed();

    /**
     * @brief Checks if the L1 button was released since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the L1 button was released, false otherwise.
     */
    bool l1WasReleased();

    /**
     * @brief Checks if the L2 button is currently pressed.
     * @return True if the L2 button is pressed, false otherwise.
     */
    bool l2IsPressed();

    /**
     * @brief Checks if the L2 button was pressed since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the L2 button was pressed, false otherwise.
     */
    bool l2WasPressed();

    /**
     * @brief Checks if the L2 button was released since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the L2 button was released, false otherwise.
     */
    bool l2WasReleased();

    /**
     * @brief Gets the value of the L2 button.
     * @return The value of the L2 button.
     */
    int l2Value();

    /**
     * @brief Checks if the PS button is currently pressed.
     * @return True if the PS button is pressed, false otherwise.
     */
    bool psIsPressed();

    /**
     * @brief Checks if the PS button was pressed since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the PS button was pressed, false otherwise.
     */
    bool psWasPressed();

    /**
     * @brief Checks if the PS button was released since the last time this function was called. This state is cleared
     * after being called, so subsequent calls will return false until the PS button is released again.
     * @return True if the PS button was released, false otherwise.
     */
    bool psWasReleased();

    /**
     * @brief Gets the X-axis value of the thumbstick.
     * @return The X-axis value of the thumbstick.
     */
    int thumbstickX();

    /**
     * @brief Gets the Y-axis value of the thumbstick.
     * @return The Y-axis value of the thumbstick.
     */
    int thumbstickY();

    /**
     * @brief Gets the normalized X-axis value of the thumbstick.
     * @return The normalized X-axis value of the thumbstick. Values range from -1.0 to 1.0.
     */
    float thumbstickXNorm();

    /**
     * @brief Gets the normalized Y-axis value of the thumbstick.
     * @return The normalized Y-axis value of the thumbstick. Values range from -1.0 to 1.0.
     */
    float thumbstickYNorm();

    /**
     * @brief Updates the state of the NavigationController.
     */
    void update();

  private:
    int _deadzone;  ///< The deadzone value for the navigation controls.

    unsigned int _lastDpadState;         ///< The state of the D-pad since the last update.
    unsigned int _DpadWasPressed;        ///< The D-pad state indicating if it was pressed since the last update.
    unsigned int _DpadWasReleased;       ///< The D-pad state indicating if it was released since the last update.
    unsigned int _lastButtonState;       ///< The state of the buttons since the last update.
    unsigned int _buttonWasPressed;      ///< The button state indicating if it was pressed since the last update.
    unsigned int _buttonWasReleased;     ///< The button state indicating if it was released since the last update.
    unsigned int _lastMiscButtonState;   ///< The state of the miscellaneous buttons since the last update.
    unsigned int _miscButtonWasPressed;  ///< The misc button state indicating if it was pressed since the last update.
    unsigned int _miscButtonWasReleased; ///< The misc button state indicating if it was released since the last update.

    GamepadPtr _controller;  ///< The gamepad associated with the NavigationController.

    /**
     * @brief Maps a value to the deadzone.
     * @param value The value to map.
     * @return The mapped value.
     */
    int _mapToDeadzone(int value);
};

#endif // NAVIGATION_CONTROLLER_HPP