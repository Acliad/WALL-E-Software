#include "recording_panel.hpp"

RecordingPanel::RecordingPanel()
    // TODO: The -10 is a hack to force the first update. This should be fixed.
    : _set_page(Page::NONE), _last_update_page(Page::NONE), _given_info({0}), _last_update_info({0}),
      _servos(nullptr) {
}

void RecordingPanel::setStartPage() {
    _set_page = Page::START;
}

void RecordingPanel::setRecordingPage(unsigned int keyframe_duration_ms, unsigned int cursor_position,
                                      unsigned int keyframe_num, ServoContext *servo_context) {
    _set_page = Page::RECORDING;
    _given_info.keyframe_duration_ms = keyframe_duration_ms;
    _given_info.keyframe_num = keyframe_num;
    _given_info.duration_cursor_position = cursor_position;
    _servos = servo_context;
}

void RecordingPanel::setSavePage() {
    _set_page = Page::SAVE;
}

void RecordingPanel::setCancelPage() {
    _set_page = Page::CANCEL;
}

void RecordingPanel::drawOn(TFT_eSPI &tft) {
    if (tft.fontLoaded) {
        tft.unloadFont(); 
    }
    tft.setTextFont(_FONT_NUMBER);

    if (_set_page != _last_update_page) {
        switch (_set_page) {
            case Page::START:
                _drawStartPage(tft);
                break;
            case Page::RECORDING:
                _drawRecordingPageStatic(tft);
                break;
            case Page::SAVE:
                _drawSavePage(tft);
                break;
            case Page::CANCEL:
                _drawCancelPage(tft);
                break;
            case Page::NONE:
                tft.fillScreen(_DISPLAY_BACKGROUND_COLOR);
                break;
        }
        _last_update_page = _set_page;
    }

    if (_set_page == Page::RECORDING) {
        _drawRecordingPageUpdate(tft);
    }
}

void RecordingPanel::drawOnForce(TFT_eSPI &tft) {
    if (tft.fontLoaded) {
        tft.unloadFont(); 
    }
    tft.setTextFont(_FONT_NUMBER);

    switch (_set_page) {
        case Page::START:
            _drawStartPage(tft);
            break;
        case Page::RECORDING:
            _drawRecordingPageStatic(tft);
            break;
        case Page::SAVE:
            _drawSavePage(tft);
            break;
        case Page::CANCEL:
            _drawCancelPage(tft);
            break;
        case Page::NONE:
            tft.fillScreen(_DISPLAY_BACKGROUND_COLOR);
            break;
        _last_update_page = _set_page;
    }

    // TODO: Add a way to force update this page
    if (_set_page == Page::RECORDING) {
        _drawRecordingPageUpdate(tft);
    }
}

void RecordingPanel::_drawStartPage(TFT_eSPI &tft) {
    tft.fillScreen(_DISPLAY_BACKGROUND_COLOR);
    tft.setCursor(_PAGE_TITLE_X, _PAGE_TITLE_Y, _FONT_NUMBER);
    tft.setTextColor(_DEFAULT_FONT_COLOR);
    tft.setTextSize(_TITLE_FONT_SIZE);
    tft.print(_START_PAGE_TITLE);

    // TODO: Update these graphics so they look good, remove magic numbers
    // ------------ Draw storage buttons -------------
    // Top
    tft.fillRoundRect(320/2-25, 50, 50, 50, 5, TFT_DARKGREY);
    // right
    tft.fillRoundRect(320-50, 120, 50, 50, 5, TFT_DARKGREY);
    // bottom
    tft.fillRoundRect(320/2-25, 190, 50, 50, 5, TFT_DARKGREY);
    // left
    tft.fillRoundRect(0, 120, 50, 50, 5, TFT_DARKGREY);
}

void RecordingPanel::_drawRecordingPageUpdate(TFT_eSPI &tft) {

        // Set font styles
        tft.setTextColor(_DEFAULT_FONT_COLOR, _DISPLAY_BACKGROUND_COLOR, true);
        tft.setTextSize(_INFORMATION_FONT_SIZE);

        _drawRecordingPageUpdateDuration(tft);

        // Draw the keyframe number
        tft.setCursor(_PAGE_MARGIN + tft.textWidth(_RECORDING_INFO_KEYFRAME_TEXT), 70, _FONT_NUMBER);
        if (_given_info.keyframe_num != _last_update_info.keyframe_num) {
            tft.printf("%-3d", _given_info.keyframe_num);
            _last_update_info.keyframe_num = _given_info.keyframe_num;
        }

        // Draw the servo positions
        int font_height = tft.fontHeight();
        int text_y = 100;
        // Update servo positions if they updated
        // NOTE: it would be very nice to make this independent of the specific servos. This could be done by giving the 
        // servo objects ID information then iterating through every servo in the servo context. This would require a
        // vector to store the positions. 
        _updateServoPositions();
        tft.setCursor(_PAGE_MARGIN + tft.textWidth("Left Eye: "), text_y, _FONT_NUMBER);
        if (_given_info.left_eye_position != _last_update_info.left_eye_position) {
            tft.printf(_SERVO_POS_TEXT_FORMATTER, _given_info.left_eye_position);
            _last_update_info.left_eye_position = _given_info.left_eye_position;
        }
        text_y += font_height;
        tft.setCursor(_PAGE_MARGIN + tft.textWidth("Right Eye: "), text_y, _FONT_NUMBER);
        if (_given_info.right_eye_position != _last_update_info.right_eye_position) {
            tft.printf(_SERVO_POS_TEXT_FORMATTER, _given_info.right_eye_position);
            _last_update_info.right_eye_position = _given_info.right_eye_position;
        }
        text_y += font_height;
        tft.setCursor(_PAGE_MARGIN + tft.textWidth("Neck Pitch: "), text_y, _FONT_NUMBER);
        if (_given_info.neck_pitch_position != _last_update_info.neck_pitch_position) {
            tft.printf(_SERVO_POS_TEXT_FORMATTER, _given_info.neck_pitch_position);
            _last_update_info.neck_pitch_position = _given_info.neck_pitch_position;
        }
        text_y += font_height;
        tft.setCursor(_PAGE_MARGIN + tft.textWidth("Neck Yaw: "), text_y, _FONT_NUMBER);
        if (_given_info.neck_yaw_position != _last_update_info.neck_yaw_position) {
            tft.printf(_SERVO_POS_TEXT_FORMATTER, _given_info.neck_yaw_position);
            _last_update_info.neck_yaw_position = _given_info.neck_yaw_position;
        }
        text_y += font_height;
        tft.setCursor(_PAGE_MARGIN + tft.textWidth("Left Shoulder: "), text_y, _FONT_NUMBER);
        if (_given_info.left_shoulder_position != _last_update_info.left_shoulder_position) {
            tft.printf(_SERVO_POS_TEXT_FORMATTER, _given_info.left_shoulder_position);
            _last_update_info.left_shoulder_position = _given_info.left_shoulder_position;
        }
        text_y += font_height;
        tft.setCursor(_PAGE_MARGIN + tft.textWidth("Right Shoulder: "), text_y, _FONT_NUMBER);
        if (_given_info.right_shoulder_position != _last_update_info.right_shoulder_position) {
            tft.printf(_SERVO_POS_TEXT_FORMATTER, _given_info.right_shoulder_position);
            _last_update_info.right_shoulder_position = _given_info.right_shoulder_position;
        }
        text_y += font_height;
        tft.setCursor(_PAGE_MARGIN + tft.textWidth("Left Elbow: "), text_y, _FONT_NUMBER);
        if (_given_info.left_elbow_position != _last_update_info.left_elbow_position) {
            tft.printf(_SERVO_POS_TEXT_FORMATTER, _given_info.left_elbow_position);
            _last_update_info.left_elbow_position = _given_info.left_elbow_position;
        }
        text_y += font_height;
        tft.setCursor(_PAGE_MARGIN + tft.textWidth("Right Elbow: "), text_y, _FONT_NUMBER);
        if (_given_info.right_elbow_position != _last_update_info.right_elbow_position) {
            tft.printf(_SERVO_POS_TEXT_FORMATTER, _given_info.right_elbow_position);
            _last_update_info.right_elbow_position = _given_info.right_elbow_position;
        }
        text_y += font_height;
        tft.setCursor(_PAGE_MARGIN + tft.textWidth("Left Wrist: "), text_y, _FONT_NUMBER);
        if (_given_info.left_wrist_position != _last_update_info.left_wrist_position) {
            tft.printf(_SERVO_POS_TEXT_FORMATTER, _given_info.left_wrist_position);
            _last_update_info.left_wrist_position = _given_info.left_wrist_position;
        }
        text_y += font_height;
        tft.setCursor(_PAGE_MARGIN + tft.textWidth("Right Wrist: "), text_y, _FONT_NUMBER);
        if (_given_info.right_wrist_position != _last_update_info.right_wrist_position) {
            tft.printf(_SERVO_POS_TEXT_FORMATTER, _given_info.right_wrist_position);
            _last_update_info.right_wrist_position = _given_info.right_wrist_position;
        }
        text_y += font_height;
        tft.setCursor(_PAGE_MARGIN + tft.textWidth("Left Hand: "), text_y, _FONT_NUMBER);
        if (_given_info.left_hand_position != _last_update_info.left_hand_position) {
            tft.printf(_SERVO_POS_TEXT_FORMATTER, _given_info.left_hand_position);
            _last_update_info.left_hand_position = _given_info.left_hand_position;
        }
        text_y += font_height;
        tft.setCursor(_PAGE_MARGIN + tft.textWidth("Right Hand: "), text_y, _FONT_NUMBER);
        if (_given_info.right_hand_position != _last_update_info.right_hand_position) {
            tft.printf(_SERVO_POS_TEXT_FORMATTER, _given_info.right_hand_position);
            _last_update_info.right_hand_position = _given_info.right_hand_position;
        }
}

void RecordingPanel::_drawRecordingPageUpdateDuration(TFT_eSPI &tft) {
    if (_given_info.keyframe_duration_ms != _last_update_info.keyframe_duration_ms ||
        _given_info.duration_cursor_position != _last_update_info.duration_cursor_position) {

        // Because we want to have a cursor over the selected digit, we need to print the duration text one
        // character at a time. The cursor position will be highlighted by swapping the background and text colors.

        // Format the duration text
        sprintf(_duration_text_buffer, _DURATION_TEXT_FORMATTER, _given_info.keyframe_duration_ms / 1000.0);
        tft.setCursor(_PAGE_MARGIN + tft.textWidth(_RECORDING_INFO_DURATION_TEXT), 40, _FONT_NUMBER);

        // Count the number of digits in the duration string, including padding
        int   duration_digits = 0;
        char *c = _duration_text_buffer;
        while (*c != '\0') {
            if (*c != '.') {
                duration_digits++;
            }
            c++;
        }

        // Print the duration text
        c = _duration_text_buffer; // Pointer to the start of the duration text
        // Index of the digit we are currently printing with the same refereance as the cursor position. I.e., the
        // last digit in the string is digit_position == 0
        int digit_position = duration_digits - 1;
        while (*c != '\0') {
            // Inverse the colors if this is the cursor position
            if (digit_position == _given_info.duration_cursor_position && *c != '.') {
                tft.setTextColor(_DISPLAY_BACKGROUND_COLOR, _DEFAULT_FONT_COLOR, true);
            } else {
                tft.setTextColor(_DEFAULT_FONT_COLOR, _DISPLAY_BACKGROUND_COLOR, true);
            }

            tft.print(*c);

            // Keep tally of the digit position (avoid the decimal point)
            if (*c != '.') {
                digit_position--;
            }
            c++;
        }
        _last_update_info.keyframe_duration_ms = _given_info.keyframe_duration_ms;
        _last_update_info.duration_cursor_position = _given_info.duration_cursor_position;
    }
}

void RecordingPanel::_drawRecordingPageStatic(TFT_eSPI &tft) {
    // TODO: Initialize all of the numbers to _given_info values
    tft.fillScreen(_DISPLAY_BACKGROUND_COLOR);
    tft.setCursor(_PAGE_TITLE_X, _PAGE_TITLE_Y, _FONT_NUMBER);
    tft.setTextColor(_DEFAULT_FONT_COLOR);
    tft.setTextSize(_TITLE_FONT_SIZE);
    tft.print(_RECORDING_PAGE_TITLE);

    tft.setTextSize(_INFORMATION_FONT_SIZE);
    // TODO: Remove magic numbers
    tft.setCursor(_PAGE_MARGIN, 70, _FONT_NUMBER);
    tft.print(_RECORDING_INFO_KEYFRAME_TEXT);
    tft.setCursor(_PAGE_MARGIN, 40, _FONT_NUMBER);
    tft.print(_RECORDING_INFO_DURATION_TEXT);

    // Draw the servo labels:
    tft.setCursor(_PAGE_MARGIN, 100, _FONT_NUMBER);
    tft.println("Left Eye: ");
    tft.println("Right Eye: ");
    tft.println("Neck Pitch: ");
    tft.println("Neck Yaw: ");
    tft.println("Left Shoulder: ");
    tft.println("Right Shoulder: ");
    tft.println("Left Elbow: ");
    tft.println("Right Elbow: ");
    tft.println("Left Wrist: ");
    tft.println("Right Wrist: ");
    tft.println("Left Hand: ");
    tft.println("Right Hand: ");
}

void RecordingPanel::_drawSavePage(TFT_eSPI &tft) {
    tft.fillScreen(_DISPLAY_BACKGROUND_COLOR);
    tft.setCursor(_PAGE_MARGIN, TFT_WIDTH/2, _FONT_NUMBER);
    tft.setTextColor(_DEFAULT_FONT_COLOR);
    tft.setTextSize(_INFORMATION_FONT_SIZE);
    tft.print(_SAVE_PAGE_TEXT);
}

void RecordingPanel::_drawCancelPage(TFT_eSPI &tft) {
    tft.fillScreen(_DISPLAY_BACKGROUND_COLOR);
    tft.setCursor(_PAGE_MARGIN, TFT_WIDTH/2, _FONT_NUMBER);
    tft.setTextColor(_DEFAULT_FONT_COLOR);
    tft.setTextSize(_INFORMATION_FONT_SIZE);
    tft.print(_CANCEL_PAGE_TEXT);
}

void RecordingPanel::_updateServoPositions() {
    _given_info.left_eye_position       = _servos->servo_eye_left->get_current_scalar();
    _given_info.right_eye_position      = _servos->servo_eye_right->get_current_scalar();

    _given_info.neck_pitch_position     = _servos->servo_neck_pitch->get_current_scalar();
    _given_info.neck_yaw_position       = _servos->servo_neck_yaw->get_current_scalar();

    _given_info.left_shoulder_position  = _servos->servo_shoulder_left->get_current_scalar();
    _given_info.right_shoulder_position = _servos->servo_shoulder_right->get_current_scalar();

    _given_info.left_elbow_position     = _servos->servo_elbow_left->get_current_scalar();
    _given_info.right_elbow_position    = _servos->servo_elbow_right->get_current_scalar();

    _given_info.left_wrist_position     = _servos->servo_wrist_left->get_current_scalar();
    _given_info.right_wrist_position    = _servos->servo_wrist_right->get_current_scalar();

    _given_info.left_hand_position      = _servos->servo_hand_left->get_current_scalar();
    _given_info.right_hand_position     = _servos->servo_hand_right->get_current_scalar();
}
