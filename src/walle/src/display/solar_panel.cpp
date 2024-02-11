#include "solar_panel.hpp"

SolarPanel::SolarPanel() : _set_state({{false}, false}), _current_state({{false}, false}){};

void SolarPanel::setBar(unsigned int index, bool on) {
    index = constrain(index, 0, _SOLAR_PANEL_NUM_BARS);
    this->_set_state.bar_status[index] = on;
}

void SolarPanel::setAllBars(bool *_bar_status) {
    // Sets all the bar statuses to the values in _bar_status.
    for (int i = 0; i < _SOLAR_PANEL_NUM_BARS; i++) {
        this->setBar(i, _bar_status[i]);
    }
}

bool SolarPanel::isBarOn(unsigned int index) {
    return this->_set_state.bar_status[index];
}

bool SolarPanel::isSunOn() {
    return this->_set_state.sun_on;
}

void SolarPanel::setSun(bool on) {
    this->_set_state.sun_on = on;
}

void SolarPanel::_drawSun(TFT_eSPI &tft) {
    // Draw the sun
    const float ray_angle_delta_rad = TWO_PI / _DISPLAY_SUN_NUM_RAYS;
    const int   ray_start_radius = _DISPLAY_SUN_INNER_DIAMETER / 2 + _DISPLAY_SUN_SPACING;
    const int   sun_center_x = _DISPLAY_MARGIN + _DISPLAY_SUN_OUTER_DIAMETER / 2;
    const int   sun_center_y = _DISPLAY_GFX_MARGIN + _DISPLAY_SUN_OUTER_DIAMETER / 2;

    tft.fillSmoothCircle(sun_center_x, sun_center_y, _DISPLAY_SUN_INNER_DIAMETER / 2, _DISPLAY_COLOR);
    tft.fillSmoothCircle(sun_center_x, sun_center_y, _DISPLAY_SUN_INNER_DIAMETER / 2 - _DISPLAY_SUN_LINE_WIDTH,
                         TFT_BLACK);

    for (int i = 0; i < _DISPLAY_SUN_NUM_RAYS; i++) {
        float        theta = ray_angle_delta_rad * i;
        unsigned int x_start = round(ray_start_radius * cos(theta)) + sun_center_x;
        unsigned int y_start = round(ray_start_radius * sin(theta)) + sun_center_y;

        unsigned int x_end = round(_DISPLAY_SUN_OUTER_DIAMETER / 2 * cos(theta)) + sun_center_x;
        unsigned int y_end = round(_DISPLAY_SUN_OUTER_DIAMETER / 2 * sin(theta)) + sun_center_y;
        tft.drawWideLine(x_start, y_start, x_end, y_end, _DISPLAY_SUN_LINE_WIDTH, _DISPLAY_COLOR);
    }
}

void SolarPanel::_blankSun(TFT_eSPI &tft) {
    tft.drawRect(_DISPLAY_MARGIN, _DISPLAY_GFX_MARGIN + _DISPLAY_SUN_OUTER_DIAMETER, _DISPLAY_SUN_OUTER_DIAMETER,
                 _DISPLAY_SUN_OUTER_DIAMETER, _DISPLAY_BACKGROUND_COLOR);
}

void SolarPanel::_drawBars(TFT_eSPI &tft, bool force_redraw) {
    // Loop through all the bars and update the ones which have changed
    for (int i = 0; i < _SOLAR_PANEL_NUM_BARS; i++) {
        if (this->_set_state.bar_status[i] != _current_state.bar_status[i] || force_redraw) {
            // Determine the size of the bar. The first bar is larger than the rest, which are all the same size
            unsigned int rect_height = (i == 0) ? _BARS_LARGE_RECT_HEIGHT : _BARS_SMALL_RECT_HEIGHT;
            unsigned int rect_width = (i == 0) ? _BARS_LARGE_RECT_WIDTH : _BARS_SMALL_RECT_WIDTH;

            unsigned int ypos =
                _DISPLAY_GFX_MARGIN + (_SOLAR_PANEL_NUM_BARS - i - 1) * (_BARS_SMALL_RECT_HEIGHT + _BARS_RECT_SPACING);

            tft.fillSmoothRoundRect(_DISPLAY_WIDTH - _DISPLAY_MARGIN - rect_width, ypos, rect_width, rect_height,
                                    _BARS_CORNER_RADIUS,
                                    _set_state.bar_status[i] ? _DISPLAY_COLOR : _DISPLAY_BACKGROUND_COLOR);

            this->_current_state.bar_status[i] = _set_state.bar_status[i];
        }
    }
}

void SolarPanel::drawOn(TFT_eSPI &tft) {
    if (!tft.fontLoaded) {
        tft.loadFont(_SOLAR_FONT_NAME);
        tft.setTextDatum(TC_DATUM); // Set strings to draw from center
        tft.setTextColor(_DISPLAY_COLOR, TFT_BLACK);
        // TODO: Move this to it's own _drawSolarText() function and create toggle
        tft.drawString("SOLAR CHARGE LEVEL", _DISPLAY_WIDTH / 2, _DISPLAY_MARGIN);
    }
    // Update the sun if changed
    if (this->_set_state.sun_on != this->_current_state.sun_on) {
        this->_set_state.sun_on ? this->_drawSun(tft) : this->_blankSun(tft);
        this->_current_state.sun_on = this->_set_state.sun_on;
    }

    // Update the bars. _drawBars() will only redraw bars that have changed
    this->_drawBars(tft, false);
}

void SolarPanel::drawOnForce(TFT_eSPI &tft) {
    if (!tft.fontLoaded) {
        tft.loadFont(_SOLAR_FONT_NAME);
        tft.setTextDatum(TC_DATUM); // Set strings to draw from center
        tft.setTextColor(_DISPLAY_COLOR, TFT_BLACK);
    }
    // TODO: Move this to it's own _drawSolarText() function and create toggle
    tft.drawString("SOLAR CHARGE LEVEL", _DISPLAY_WIDTH / 2, _DISPLAY_MARGIN);
    // Force a redraw of the solar panel
    this->_drawSun(tft);
    this->_drawBars(tft, true);

    // Update the current state to match
    for (int i = 0; i < _SOLAR_PANEL_NUM_BARS; i++) {
        this->_current_state.bar_status[i] = _set_state.bar_status[i];
    }
    this->_current_state.sun_on = _set_state.sun_on;
}