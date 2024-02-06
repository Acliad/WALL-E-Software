#include "display.hpp"


Display::Display(TFT_eSPI& tft)
    : tft(tft), solar_panel(), animation() {
    }

void Display::setBar(unsigned int index, bool on) {
    // animation is active, don't update the display. This forces the user to explicitly stop the animation. 
    if(!this->animation.isRunning()) {
        this->solar_panel.setBar(index, on);
    }
}

void Display::begin() {
    this->tft.begin();
    // Make sure things are cleared
    this->tft.fillScreen(_DISPLAY_BACKGROUND_COLOR);
    this->_draw_static_display_elements();
}

bool Display::isBarOn(unsigned int index) {
    return this->solar_panel.isBarOn(index);
}

void Display::update() {
    this->animation.update();
    this->solar_panel.drawOn(this->tft);
}

void Display::setAnimation(AnimateSolarPanel animation) {
    animation.setSolarPanel(&this->solar_panel);
    this->animation = animation;
}

AnimateSolarPanel Display::getAnimation() {
    return this->animation;
}

void Display::startAnimation() {
    this->animation.start();
}

void Display::stopAnimation() {
    this->animation.stop();
}

bool Display::isAnimationRunning() {
    return this->animation.isRunning();
}

// TODO: Move the solar specifc stuff to a solar display class (currently solar_panel)
void Display::_draw_static_display_elements() {
    // Draw the SOLAR CHARGE LEVEL text
    this->tft.setTextSize(_DISPLAY_FONT_SIZE);
    this->tft.setTextColor(_DISPLAY_COLOR, TFT_BLACK);
    this->tft.loadFont(_DISPLAY_FONT_NAME);
    this->tft.setTextDatum(TC_DATUM); // Set strings to draw from center
    this->tft.drawString("SOLAR CHARGE LEVEL", _DISPLAY_WIDTH/2, _DISPLAY_MARGIN);
}
