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

// TODO: Move the solar specifc stuff to a solar display class (currently solar_bars)
void SolarDisplay::_draw_static_display_elements() {
    // Draw the SOLAR CHARGE LEVEL text
    this->tft.setTextSize(_DISPLAY_FONT_SIZE);
    this->tft.setTextColor(_DISPLAY_COLOR, TFT_BLACK);
    this->tft.loadFont(_DISPLAY_FONT_NAME);
    this->tft.setTextDatum(TC_DATUM); // Set strings to draw from center
    this->tft.drawString("SOLAR CHARGE LEVEL", _DISPLAY_WIDTH/2, _DISPLAY_MARGIN);
}
