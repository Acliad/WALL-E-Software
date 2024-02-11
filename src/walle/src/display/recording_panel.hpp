#ifndef RECORDING_PANEL_HPP
#define RECORDING_PANEL_HPP

#include <TFT_eSPI.h>
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
        float left_eye_position;
        float right_eye_position;
        float neck_pitch_position;
        float neck_yaw_position;
        float left_shoulder_position;
        float right_shoulder_position;
        float left_elbow_position;
        float right_elbow_position;
        float left_wrist_position;
        float right_wrist_position;
        float left_hand_position;
        float right_hand_position;
    };

    const int _PAGE_MARGIN = 0;
    const int _FONT_NUMBER = 1;
    const int _PAGE_TITLE_X = _PAGE_MARGIN;
    const int _PAGE_TITLE_Y = _PAGE_MARGIN;
    const int _TITLE_FONT_SIZE = 3;
    const int _INFORMATION_FONT_SIZE = 2;
    const int _DEFAULT_FONT_COLOR = TFT_GREEN;
    const char* _DURATION_TEXT_FORMATTER = "%7.3f"; // 999.999 == 7 chars
    const char* _SERVO_POS_TEXT_FORMATTER = "%-5.2f"; // -x.00 == 5 chars

    const char* _START_PAGE_TITLE = "Select Storage Button to Start Recording";
    const char* _RECORDING_PAGE_TITLE = "Recording...";
    const char* _CANCEL_PAGE_TEXT = "Do you want to cancel recording? Press again to confirm.";
    const char* _SAVE_PAGE_TEXT = "Do you want to save recording? Press again to confirm.";
    const char* _RECORDING_INFO_KEYFRAME_TEXT = "Keyframe: ";
    const char* _RECORDING_INFO_DURATION_TEXT = "Duration (sec): ";

    Page _set_page;
    Page _last_update_page;
    recording_page_info _given_info;
    recording_page_info _last_update_info;
    // NOTE: this is a buffer little space ineffecient and there's a magic number, but the cost is small and this is
    // straightforward
    char _duration_text_buffer[20] = {0}; 

    ServoContext *_servos;

    void _drawStartPage(TFT_eSPI &tft);
    void _drawRecordingPageUpdate(TFT_eSPI &tft);
    void _drawRecordingPageUpdateDuration(TFT_eSPI &tft);
    void _drawRecordingPageStatic(TFT_eSPI &tft);
    void _drawSavePage(TFT_eSPI &tft);
    void _drawCancelPage(TFT_eSPI &tft);
    void _updateServoPositions();
};

#endif // RECORDING_PANEL_HPP
