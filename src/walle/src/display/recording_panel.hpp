/**
 * @file recording_panel.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file defines the RecordingPanel class, which is used to control the display of the during animation
 * recording.
 * @version 0.1
 * @date 2024-02-19
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef RECORDING_PANEL_HPP
#define RECORDING_PANEL_HPP

#include <TFT_eSPI.h>
#include <map>
#include "display_common.hpp"
#include "../motion/servo_context.hpp"

/**
 * @class RecordingPanel
 * @brief Represents a panel for recording and managing keyframes.
 * 
 * The RecordingPanel class provides functionality for setting different pages, drawing on a TFT display,
 * and managing keyframes during recording. It also handles servo positions and user input controls.
 */
class RecordingPanel {
public:
    /**
     * @brief Default constructor for the RecordingPanel class.
     */
    RecordingPanel();

    /**
     * @brief Set the start page of the recording panel.
     */
    void setStartPage();

    /**
     * @brief Set the recording page of the recording panel.
     * 
     * @param keyframe_duration_ms The duration of each keyframe in milliseconds.
     * @param cursor_position The current cursor position.
     * @param keyframe_num The current keyframe number.
     * @param servo_context A pointer to the ServoContext object.
     */
    void setRecordingPage(unsigned int keyframe_duration_ms, unsigned int cursor_position, unsigned int keyframe_num,
                          ServoContext *servo_context);

    /**
     * @brief Set the save page of the recording panel.
     */
    void setSavePage();

    /**
     * @brief Set the cancel page of the recording panel.
     */
    void setCancelPage();

    /**
     * @brief Draw the recording panel on the TFT display.
     * 
     * @param tft The TFT_eSPI object representing the TFT display.
     */
    void drawOn(TFT_eSPI &tft);

    /**
     * @brief Draw the recording panel on the TFT display, forcing a redraw of all elements.
     * 
     * @param tft The TFT_eSPI object representing the TFT display.
     */
    void drawOnForce(TFT_eSPI &tft);


private:
    /**
     * @brief Enum class representing the different pages of the recording panel.
     */
    enum class Page {
        START,    /**< The start page. */
        RECORDING,    /**< The recording page. */
        SAVE,    /**< The save page. */
        CANCEL,    /**< The cancel page. */
        NONE    /**< No page. */
    };

    /**
     * @brief Struct representing the information of the recording page.
     */
    struct recording_page_info {
        int keyframe_duration_ms;    /**< The duration of each keyframe in milliseconds. */
        int keyframe_num;    /**< The current keyframe number. */
        int duration_cursor_position;    /**< The current cursor position. */
        std::map<std::string, float> servo_positions;    /**< The positions of the servos. */
    };

    const int _MARGIN = 0;
    const int _FONT_NUMBER = 1;
    const int _PAGE_TITLE_X = _MARGIN;
    const int _PAGE_TITLE_Y = _MARGIN;
    const int _TITLE_FONT_SIZE = 3;
    const int _INFORMATION_FONT_SIZE = 2;
    const int _LEADING_MEDIUM_PIXELS = 8;
    const int _LEADING_SMALL_PIXELS = 4;
    const int _FONT_COLOR_A = 0x1D7F;    /**< 16-bit color; EVE's eye blue (or close enough) */
    const int _FONT_COLOR_B = 0xFFFF;    /**< 16-bit color; White */
    const int _DPAD_BUTTON_HEIGHT = 98;
    const int _DPAD_BUTTON_WIDTH = 70;
    const int _DPAD_BUTTON_RECT_RADIUS = 5;
    const float _DPAD_SPACING_SCALER = 0.2;            /**< Spacing is scaled by the height of the button */
    const char* _DURATION_VALUE_FORMATTER = "%7.3f";   /**< xxx.xxx == 7 characters */
    const char* _SERVO_POS_VALUE_FORMATTER = "%-5.2f"; /**< -1.00 == 5 characters, left aligned */
    const char* _KEYFRAME_VALUE_FORMATTER = "%-3d";    /**< 999 == 3 characters, left aligned */

    const char *_CONTROLS_TEXT = "Primary Controller:\n"
                                 "  X: Previous Keyframe\n"
                                 "  O: Next Keyframe\n"
                                 "  D-Pad: Adjust duration\n"
                                 "  Thumbstick: Del Keyframe\n"
                                 "Secondary Controller:\n"
                                 "  X: Cancel Recording\n"
                                 "  O: Save Recording\n"
                                 "  D-Pad: Bind sound\n"
                                 "  Thumbstick: Remove sound";
    const char* _START_PAGE_TITLE = "Select Storage \nButton to Start \nRecording...";
    const char* _RECORDING_PAGE_TITLE = "Recording...";
    const char* _PRESS_TO_CONFIRM_TEXT = "Press again to confirm.\n\nPress any other button to go back.";
    const char* _SAVE_PAGE_TITLE = "Saving...";
    const char* _CANCEL_PAGE_TITLE = "Canceling...";
    const char* _RECORDING_INFO_KEYFRAME_TEXT = "Keyframe: ";
    const char* _RECORDING_INFO_DURATION_TEXT = "Duration (sec): ";

    // Define the order of the servos to be displayed
    const char* _SERVO_DISPLAY_ORDER[12] = {
        SERVO_EYE_LEFT_NAME,
        SERVO_EYE_RIGHT_NAME,
        SERVO_NECK_PITCH_NAME,
        SERVO_NECK_YAW_NAME,
        SERVO_SHOULDER_LEFT_NAME,
        SERVO_SHOULDER_RIGHT_NAME,
        SERVO_ELBOW_LEFT_NAME,
        SERVO_ELBOW_RIGHT_NAME,
        SERVO_WRIST_LEFT_NAME,
        SERVO_WRIST_RIGHT_NAME,
        SERVO_HAND_LEFT_NAME,
        SERVO_HAND_RIGHT_NAME
    };

    Page _set_page;                        /**< The currently set page. */
    Page _last_update_page;                /**< The page that was set on the last update. */
    recording_page_info _given_info;       /**< The given information of the recording page. */
    recording_page_info _last_update_info; /**< The information used on the last update of the recording page. */
    char _duration_text_buffer[20] = {0};  /**< Buffer for duration text. */
    ServoContext *_servos;                 /**< Pointer to the ServoContext object. */

    int _keyframe_info_y;    /**< The y-coordinate of the keyframe information. */
    int _duration_info_y;    /**< The y-coordinate of the duration information. */
    int _servo_info_start_y; /**< The starting y-coordinate of the servo information. */

    bool _force_redraw; /**< Flag indicating whether to force a redraw. */

    /**
     * @brief Draw the start page on the TFT display.
     * 
     * @param tft The TFT_eSPI object representing the TFT display.
     */
    void _drawStartPage(TFT_eSPI &tft);

    /**
     * @brief Update and draw the recording page on the TFT display.
     * 
     * @param tft The TFT_eSPI object representing the TFT display.
     * @param force_redraw Flag indicating whether to force a redraw of all elements.
     */
    void _drawRecordingPageUpdate(TFT_eSPI &tft, bool force_redraw);

    /**
     * @brief Draw the duration element of the recording page on the TFT display.
     * 
     * @param tft The TFT_eSPI object representing the TFT display.
     * @param force_redraw Flag indicating whether to force a redraw of all elements.
     */
    void _drawRecordingPageDuration(TFT_eSPI &tft, bool force_redraw);

    /**
     * @brief Draw the static elements of the recording page on the TFT display.
     * 
     * @param tft The TFT_eSPI object representing the TFT display.
     */
    void _drawRecordingPageStatic(TFT_eSPI &tft);

    /**
     * @brief Draw the save page on the TFT display.
     * 
     * @param tft The TFT_eSPI object representing the TFT display.
     */
    void _drawSavePage(TFT_eSPI &tft);

    /**
     * @brief Draw the cancel page on the TFT display.
     * 
     * @param tft The TFT_eSPI object representing the TFT display.
     */
    void _drawCancelPage(TFT_eSPI &tft);

    /**
     * @brief Draw the positions of the servos on the TFT display.
     * 
     * @param tft The TFT_eSPI object representing the TFT display.
     * @param force_update Flag indicating whether to force an update of the servo positions.
     */
    void _drawServoPositions(TFT_eSPI &tft, bool force_update);

    /**
     * @brief Draw the D-Pad button sprites on the TFT display.
     * 
     * @param tft The TFT_eSPI object representing the TFT display.
     */
    void _drawDPadButtonSprites(TFT_eSPI &tft);
};

#endif // RECORDING_PANEL_HPP
