#include "recording_panel.hpp"

RecordingPanel::RecordingPanel()
    : _set_page(Page::NONE), _last_update_page(Page::NONE), _given_info({0}), _last_update_info({0}), _servos(nullptr),
      _force_redraw(false), _keyframe_info_y(0), _duration_info_y(0), _servo_info_start_y(0) {
}

void RecordingPanel::setStartPage() {
    _set_page = Page::START;
}

void RecordingPanel::setRecordingPage(unsigned int keyframe_duration_ms, unsigned int cursor_position,
                                      unsigned int keyframe_num, ServoContext *servo_context) {
    // If we were previously on a different page from the recording page, we need to force a redraw to update the
    // dynamic values.
    _force_redraw = _set_page != Page::RECORDING;
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
        _drawRecordingPageUpdate(tft, _force_redraw);
        _force_redraw = false;
    }
}

void RecordingPanel::drawOnForce(TFT_eSPI &tft) {
    _force_redraw = true;
    drawOn(tft);
}

void RecordingPanel::_drawStartPage(TFT_eSPI &tft) {
    // Blank the scren
    tft.fillScreen(_DISPLAY_BACKGROUND_COLOR);

    /*----------- Title ---------------------------------*/
    tft.setCursor(_PAGE_TITLE_X, _PAGE_TITLE_Y, _FONT_NUMBER);
    tft.setTextColor(_FONT_COLOR_A);
    tft.setTextSize(_TITLE_FONT_SIZE);
    tft.print(_START_PAGE_TITLE);

    /*----------- Draw storage buttons ------------------*/
    _drawDPadButtonSprites(tft);
}

void RecordingPanel::_drawRecordingPageUpdate(TFT_eSPI &tft, bool force_update) {

    /*----------- Keyframe number -----------------------*/
    tft.setTextColor(_FONT_COLOR_B, _DISPLAY_BACKGROUND_COLOR, true);
    tft.setTextSize(_INFORMATION_FONT_SIZE);
    tft.setCursor(_MARGIN + tft.textWidth(_RECORDING_INFO_KEYFRAME_TEXT), _keyframe_info_y, _FONT_NUMBER);
    if (_given_info.keyframe_num != _last_update_info.keyframe_num || force_update) {
        tft.printf(_KEYFRAME_VALUE_FORMATTER, _given_info.keyframe_num);
        _last_update_info.keyframe_num = _given_info.keyframe_num;
    }

    /*----------- Duration ------------------------------*/
    _drawRecordingPageDuration(tft, force_update);

    /*----------- Servos --------------------------------*/
    _drawServoPositions(tft, force_update);
}

void RecordingPanel::_drawRecordingPageDuration(TFT_eSPI &tft, bool force_update) {
    if (_given_info.keyframe_duration_ms != _last_update_info.keyframe_duration_ms ||
        _given_info.duration_cursor_position != _last_update_info.duration_cursor_position || force_update) {
        // Because we want to have a cursor over the selected digit, we need to print the duration text one
        // character at a time. The cursor position will be highlighted by swapping the background and text colors.

        // Format the duration text
        sprintf(_duration_text_buffer, _DURATION_VALUE_FORMATTER, _given_info.keyframe_duration_ms / 1000.0);
        tft.setCursor(_MARGIN + tft.textWidth(_RECORDING_INFO_DURATION_TEXT), 40, _FONT_NUMBER);

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
                tft.setTextColor(_DISPLAY_BACKGROUND_COLOR, _FONT_COLOR_B, true);
            } else {
                tft.setTextColor(_FONT_COLOR_B, _DISPLAY_BACKGROUND_COLOR, true);
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
    // Blank the screen
    tft.fillScreen(_DISPLAY_BACKGROUND_COLOR);

    /*----------- Title ---------------------------------*/
    tft.setCursor(_PAGE_TITLE_X, _PAGE_TITLE_Y, _FONT_NUMBER);
    tft.setTextColor(_FONT_COLOR_A);
    tft.setTextSize(_TITLE_FONT_SIZE);
    tft.println(_RECORDING_PAGE_TITLE);

    /*----------- Keyframe Number -----------------------*/
    tft.setTextSize(_INFORMATION_FONT_SIZE);
    // Adjust the cursor position to be below the title plus the leading pixels
    _keyframe_info_y = tft.getCursorY() + _LEADING_SMALL_PIXELS;
    tft.setCursor(tft.getCursorX(), _keyframe_info_y, _FONT_NUMBER);
    tft.println(_RECORDING_INFO_KEYFRAME_TEXT);

    /*----------- Keyframe Duration ---------------------*/
    _duration_info_y = tft.getCursorY() + _LEADING_SMALL_PIXELS;
    tft.setCursor(tft.getCursorX(), _duration_info_y, _FONT_NUMBER);
    tft.println(_RECORDING_INFO_DURATION_TEXT);

    /*----------- Servos --------------------------------*/
    _servo_info_start_y = tft.getCursorY() + _LEADING_MEDIUM_PIXELS;
    tft.setCursor(tft.getCursorX(), _servo_info_start_y, _FONT_NUMBER);
    for (auto servo_name : _SERVO_DISPLAY_ORDER) {
        tft.print(servo_name);
        tft.println(": ");
        tft.setCursor(tft.getCursorX(), tft.getCursorY() + _LEADING_SMALL_PIXELS, _FONT_NUMBER);
    }

    /*----------- Controller Instructions ---------------*/
    tft.setCursor(tft.getCursorX(), tft.getCursorY() + _LEADING_MEDIUM_PIXELS, _FONT_NUMBER);
    tft.println(_CONTROLS_TEXT);
}

void RecordingPanel::_drawSavePage(TFT_eSPI &tft) {
    // Blank the screen
    tft.fillScreen(_DISPLAY_BACKGROUND_COLOR);

    // Draw the title
    tft.setTextSize(_TITLE_FONT_SIZE);
    tft.setCursor(_PAGE_TITLE_X, _PAGE_TITLE_Y, _FONT_NUMBER);
    tft.setTextColor(_FONT_COLOR_A);
    tft.println(_SAVE_PAGE_TITLE);

    // Draw the save text
    tft.setTextSize(_INFORMATION_FONT_SIZE);
    tft.setCursor(_MARGIN, TFT_WIDTH / 2, _FONT_NUMBER);
    tft.println(_PRESS_TO_CONFIRM_TEXT);
}

void RecordingPanel::_drawCancelPage(TFT_eSPI &tft) {
    // Blank the screen
    tft.fillScreen(_DISPLAY_BACKGROUND_COLOR);

    // Draw the title
    tft.setTextSize(_TITLE_FONT_SIZE);
    tft.setCursor(_PAGE_TITLE_X, _PAGE_TITLE_Y, _FONT_NUMBER);
    tft.setTextColor(_FONT_COLOR_A);
    tft.println(_CANCEL_PAGE_TITLE);

    // Draw the save text
    tft.setTextSize(_INFORMATION_FONT_SIZE);
    tft.setCursor(_MARGIN, TFT_WIDTH / 2, _FONT_NUMBER);
    tft.println(_PRESS_TO_CONFIRM_TEXT);
}

void RecordingPanel::_drawServoPositions(TFT_eSPI &tft, bool force_update) {
    for (auto servo_name : _SERVO_DISPLAY_ORDER) {
        _given_info.servo_positions[servo_name] = _servos->map[servo_name]->get_current_scalar();
    }

    tft.setTextColor(_FONT_COLOR_B, _DISPLAY_BACKGROUND_COLOR, true);
    int text_y = _servo_info_start_y;
    int font_height = tft.fontHeight(_FONT_NUMBER);
    for (auto servo_name : _SERVO_DISPLAY_ORDER) {
        // Set the curosor to the end of the servo name label
        tft.setCursor(_MARGIN + tft.textWidth(servo_name) + tft.textWidth(": "), text_y, _FONT_NUMBER);

        // Print the servo position if it has changed or if we are forcing an update
        if (_given_info.servo_positions[servo_name] != _last_update_info.servo_positions[servo_name] || force_update) {
            tft.printf(_SERVO_POS_VALUE_FORMATTER, _given_info.servo_positions[servo_name]);
            _last_update_info.servo_positions[servo_name] = _given_info.servo_positions[servo_name];
        }
        text_y += font_height + _LEADING_SMALL_PIXELS;
    }
}

void RecordingPanel::_drawDPadButtonSprites(TFT_eSPI &tft) {
    TFT_eSprite button_sprite = TFT_eSprite(&tft);
    button_sprite.setColorDepth(1); // Binary color
    button_sprite.createSprite(_DPAD_BUTTON_WIDTH, _DPAD_BUTTON_HEIGHT);
    button_sprite.fillSprite(_DISPLAY_BACKGROUND_COLOR);

    // Draw the triangle bit of the D-Pad
    int triangle_start_y = _DPAD_BUTTON_HEIGHT - _DPAD_BUTTON_HEIGHT * .35;
    button_sprite.fillTriangle(0, triangle_start_y, _DPAD_BUTTON_WIDTH, triangle_start_y, _DPAD_BUTTON_WIDTH / 2,
                               _DPAD_BUTTON_HEIGHT, _FONT_COLOR_B);
    // Draw the rectangular bit of the D-Pad
    button_sprite.fillRoundRect(0, 0, _DPAD_BUTTON_WIDTH, triangle_start_y, _DPAD_BUTTON_RECT_RADIUS, _FONT_COLOR_B);
    button_sprite.fillRect(0, triangle_start_y - _DPAD_BUTTON_RECT_RADIUS, 
                           _DPAD_BUTTON_WIDTH, _DPAD_BUTTON_RECT_RADIUS,
                           _FONT_COLOR_B);

    // Draw the triangle knockout inside the D-Pad
    int triangle_knockout_x_padding = _DPAD_BUTTON_WIDTH * 0.3;
    int triangle_knockout_y_padding = triangle_start_y * 0.1;
    int triangle_knockout_height = triangle_start_y * 0.2;

    int triangle_knockout_x0 = _DPAD_BUTTON_WIDTH/2;
    int triangle_knockout_y0 = triangle_knockout_y_padding;
    int triangle_knockout_x1 = _DPAD_BUTTON_WIDTH - triangle_knockout_x_padding;
    int triangle_knockout_y1 = triangle_knockout_height + triangle_knockout_y_padding;
    int triangle_knockout_x2 = triangle_knockout_x_padding;
    int triangle_knockout_y2 = triangle_knockout_height + triangle_knockout_y_padding;
    button_sprite.fillTriangle( triangle_knockout_x0, triangle_knockout_y0,
                                triangle_knockout_x1, triangle_knockout_y1,
                                triangle_knockout_x2, triangle_knockout_y2, 
                               _DISPLAY_BACKGROUND_COLOR);

    // Draw the sprite
    int button_spacing = _DPAD_BUTTON_HEIGHT * _DPAD_SPACING_SCALER; // Spacing between the points of the buttons
    int x_pos_start = _DISPLAY_WIDTH / 2 - _DPAD_BUTTON_WIDTH / 2;
    int y_pos_start = _DISPLAY_HEIGHT / 2 - _DPAD_BUTTON_HEIGHT - button_spacing;
    int x_pos;
    int y_pos;
    button_sprite.setPivot(0, 0);
    button_sprite.setBitmapColor(_FONT_COLOR_B, _DISPLAY_BACKGROUND_COLOR);

    // Top
    x_pos = x_pos_start;
    y_pos = y_pos_start;
    button_sprite.pushSprite(x_pos, y_pos);

    // Right
    x_pos = x_pos_start + _DPAD_BUTTON_HEIGHT + button_spacing + _DPAD_BUTTON_WIDTH / 2;
    y_pos = y_pos_start + _DPAD_BUTTON_HEIGHT + button_spacing - _DPAD_BUTTON_WIDTH / 2;
    tft.setPivot(x_pos, y_pos);
    button_sprite.pushRotated(90);

    // Down
    x_pos = x_pos_start + _DPAD_BUTTON_WIDTH;
    y_pos = y_pos_start + _DPAD_BUTTON_HEIGHT * 2 + button_spacing * 2;
    tft.setPivot(x_pos, y_pos);
    button_sprite.pushRotated(180);

    // Left
    x_pos = x_pos_start - _DPAD_BUTTON_HEIGHT - button_spacing + _DPAD_BUTTON_WIDTH / 2;
    y_pos = y_pos_start + _DPAD_BUTTON_HEIGHT + button_spacing + _DPAD_BUTTON_WIDTH / 2;
    tft.setPivot(x_pos, y_pos);
    button_sprite.pushRotated(270);

    button_sprite.deleteSprite();
}
