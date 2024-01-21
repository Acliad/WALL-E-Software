#include "display.hpp"


SolarDisplay::SolarDisplay(TFT_eSPI& tft)
    : tft(tft), solar_bars(tft), animation() {
    }

void SolarDisplay::setBar(unsigned int index, bool on) {
    // animation is active, don't update the display. This forces the user to explicitly stop the animation. 
    if(!this->animation.isRunning()) {
        this->solar_bars.setBar(index, on);
    }
}

void SolarDisplay::begin() {
    this->tft.begin();
    // Make sure things are cleared
    this->tft.fillScreen(_DISPLAY_BACKGROUND_COLOR);
    this->_draw_static_display_elements();
}

bool SolarDisplay::isBarOn(unsigned int index) {
    return this->solar_bars.isBarOn(index);
}

void SolarDisplay::update() {
    this->animation.update();
}

void SolarDisplay::setAnimation(AnimateLevel animation) {
    animation.setBars(&this->solar_bars);
    this->animation = animation;
}

AnimateLevel SolarDisplay::getAnimation() {
    return this->animation;
}

void SolarDisplay::startAnimation() {
    this->animation.start();
}

void SolarDisplay::stopAnimation() {
    this->animation.stop();
}

bool SolarDisplay::isAnimationRunning() {
    return this->animation.isRunning();
}

void SolarDisplay::_draw_static_display_elements() {
    // Draw the SOLAR CHARGE LEVEL text
    this->tft.setTextSize(_DISPLAY_FONT_SIZE);
    this->tft.setTextColor(_DISPLAY_COLOR, TFT_BLACK);
    this->tft.loadFont(_DISPLAY_FONT_NAME);
    this->tft.setTextDatum(TC_DATUM); // Set strings to draw from center
    this->tft.drawString("SOLAR CHARGE LEVEL", _DISPLAY_WIDTH/2, _DISPLAY_MARGIN);

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
