#ifndef RECORDING_PANEL_HPP
#define RECORDING_PANEL_HPP

#include <TFT_eSPI.h>
#include <map>
#include "display_common.hpp"
#include "../motion/servo_context.hpp"

class RecordingPanel {
public:
    RecordingPanel();

    void setStartPage();
    void setRecordingPage(unsigned int keyframe_duration_ms, unsigned int cursor_position, unsigned int keyframe_num,
                          ServoContext *servo_context);
    void setSavePage();
    void setCancelPage();

    void drawOn(TFT_eSPI &tft);
    void drawOnForce(TFT_eSPI &tft);


private:
    enum class Page {
        START,
        RECORDING,
        SAVE,
        CANCEL,
        NONE
    };
    struct recording_page_info {
        int keyframe_duration_ms;
        int keyframe_num;
        int duration_cursor_position;
        std::map<std::string, float> servo_positions;
    };

    const int _MARGIN = 0;
    const int _FONT_NUMBER = 1;
    const int _PAGE_TITLE_X = _MARGIN;
    const int _PAGE_TITLE_Y = _MARGIN;
    const int _TITLE_FONT_SIZE = 3;
    const int _INFORMATION_FONT_SIZE = 2;
    const int _LEADING_MEDIUM_PIXELS = 8;
    const int _LEADING_SMALL_PIXELS = 4;
    const int _FONT_COLOR_A = 0x1D7F; // 16-bit color; EVE's eye blue (or close enough)
    const int _FONT_COLOR_B = 0xFFFF; // 16-bit color; White
    const int _DPAD_BUTTON_HEIGHT = 98;
    const int _DPAD_BUTTON_WIDTH = 70;
    const int _DPAD_BUTTON_RECT_RADIUS = 5;
    const float _DPAD_SPACING_SCALER = 0.2; // Spacing is scaled by the height of the button
    const char* _DURATION_VALUE_FORMATTER = "%7.3f"; // 999.999 == 7 chars
    const char* _SERVO_POS_VALUE_FORMATTER = "%-5.2f"; // -x.00 == 5 chars
    const char* _KEYFRAME_VALUE_FORMATTER = "%-3d"; // 999 == 3 chars

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

    Page _set_page;
    Page _last_update_page;
    recording_page_info _given_info;
    recording_page_info _last_update_info;
    // NOTE: this is a buffer little space ineffecient and there's a magic number, but the cost is small and this is
    // straightforward
    char _duration_text_buffer[20] = {0}; 
    ServoContext *_servos;

    int _keyframe_info_y;
    int _duration_info_y;
    int _servo_info_start_y;

    bool _force_redraw;

    void _drawStartPage(TFT_eSPI &tft);
    void _drawRecordingPageUpdate(TFT_eSPI &tft, bool force_redraw);
    void _drawRecordingPageDuration(TFT_eSPI &tft, bool force_redraw);
    void _drawRecordingPageStatic(TFT_eSPI &tft);
    void _drawSavePage(TFT_eSPI &tft);
    void _drawCancelPage(TFT_eSPI &tft);
    void _drawServoPositions(TFT_eSPI &tft, bool force_update);
    void _drawDPadButtonSprites(TFT_eSPI &tft);
};

#endif // RECORDING_PANEL_HPP
