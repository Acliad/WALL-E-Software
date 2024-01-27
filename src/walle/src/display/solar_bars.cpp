#include "solar_bars.hpp"

SolarBars::SolarBars(TFT_eSPI& tft) : bar_status(), tft(tft), sun_on(false) {

}

void SolarBars::begin() {
    // Initialize the display for use
    this->tft.begin();
}


void SolarBars::setBar(unsigned int index, bool on) {
    index = constrain(index, 0, _BARS_NUM_BARS);


    if(this->bar_status[index] != on) {
        unsigned int rect_height = (index == 0) ? _BARS_LARGE_RECT_HEIGHT : _BARS_SMALL_RECT_HEIGHT;
        unsigned int rect_width  = (index == 0) ? _BARS_LARGE_RECT_WIDTH  : _BARS_SMALL_RECT_WIDTH;
        unsigned int ypos = _DISPLAY_GFX_MARGIN + 
                                (_BARS_NUM_BARS-index-1)*(_BARS_SMALL_RECT_HEIGHT + _BARS_RECT_SPACING);

        tft.fillSmoothRoundRect(_DISPLAY_WIDTH - _DISPLAY_MARGIN - rect_width, 
                                ypos, 
                                rect_width, 
                                rect_height, 
                                _BARS_CORNER_RADIUS, 
                                on ? _DISPLAY_COLOR : _DISPLAY_BACKGROUND_COLOR
                                );
        this->bar_status[index] = on;
    }
}

void SolarBars::setAllBars(bool *bar_status) {
    // Sets all the bar statuses to the values in bar_status. 
    for(int i = 0; i < _BARS_NUM_BARS; i++) {
        this->setBar(i, bar_status[i]);
    }
}

bool SolarBars::isBarOn(unsigned int index) { 
    return this->bar_status[index]; 
}

void SolarBars::setSun(bool on) {
    if (this->sun_on == on) {
        // State has not changed
        return;
    }

    this->sun_on = on;
    if(on) {
        // State has changed and the sun is now on
        this->_drawSun();
    } else {
        // State has changed and the sun is now off
        this->_blankSun();
    }
}   

void SolarBars::_drawSun() {
    // Draw the sun
    const float ray_angle_delta_rad = TWO_PI/_DISPLAY_SUN_NUM_RAYS;
    const int ray_start_radius = _DISPLAY_SUN_INNER_DIAMETER/2 + _DISPLAY_SUN_SPACING;
    const int sun_center_x = _DISPLAY_MARGIN + _DISPLAY_SUN_OUTER_DIAMETER/2;
    const int sun_center_y = _DISPLAY_GFX_MARGIN + _DISPLAY_SUN_OUTER_DIAMETER/2;

    tft.fillSmoothCircle(sun_center_x, sun_center_y, _DISPLAY_SUN_INNER_DIAMETER/2, _DISPLAY_COLOR);
    tft.fillSmoothCircle(sun_center_x, sun_center_y, _DISPLAY_SUN_INNER_DIAMETER/2-_DISPLAY_SUN_LINE_WIDTH, TFT_BLACK);

    for(int i = 0; i < _DISPLAY_SUN_NUM_RAYS; i++) {
        float theta = ray_angle_delta_rad * i;
        unsigned int x_start = round(ray_start_radius * cos(theta)) + sun_center_x;
        unsigned int y_start = round(ray_start_radius * sin(theta)) + sun_center_y;

        unsigned int x_end = round(_DISPLAY_SUN_OUTER_DIAMETER/2 * cos(theta)) + sun_center_x;
        unsigned int y_end = round(_DISPLAY_SUN_OUTER_DIAMETER/2 * sin(theta)) + sun_center_y;
        tft.drawWideLine(x_start, 
                        y_start, 
                        x_end, 
                        y_end, 
                        _DISPLAY_SUN_LINE_WIDTH, 
                        _DISPLAY_COLOR);
    }
}

void SolarBars::_blankSun() {
    tft.drawRect(_DISPLAY_MARGIN, 
                 _DISPLAY_GFX_MARGIN + _DISPLAY_SUN_OUTER_DIAMETER,
                 _DISPLAY_SUN_OUTER_DIAMETER,
                 _DISPLAY_SUN_OUTER_DIAMETER,
                 _DISPLAY_BACKGROUND_COLOR);
}

bool SolarBars::isSunOn() {
    return this->sun_on;
}
