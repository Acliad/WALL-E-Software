/**
 * @file display.cpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the implementation of the Display class, which is used to control the display of the solar
 * panel and recording panel.
 * @version 0.1
 * @date 2024-02-19
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "display.hpp"


Display::Display(TFT_eSPI& tft)
    : tft(tft), solar_panel(), recording_panel(), animation(), _mode(Mode::SOLAR_PANEL), _force_next_update(false) {
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
}

bool Display::isBarOn(unsigned int index) {
    return this->solar_panel.isBarOn(index);
}

void Display::update() {
    this->animation.update();
    switch (this->_mode) {
    case Mode::SOLAR_PANEL:
        if (this->_force_next_update) {
            this->solar_panel.drawOnForce(this->tft);
        } else {
            this->solar_panel.drawOn(this->tft);
        }
        break;
    case Mode::RECORDER:
        if (this->_force_next_update) {
            this->recording_panel.drawOnForce(this->tft);
        } else {
            this->recording_panel.drawOn(this->tft);
        }
        break;
    }
    this->_force_next_update = false;
}

void Display::setAnimation(AnimateSolarPanel animation) {
    animation.setSolarPanel(&this->solar_panel);
    this->animation = animation;
}

AnimateSolarPanel& Display::getAnimation() {
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


void Display::setMode(Mode mode) {
    this->_mode = mode;
    // Blank the screen when switching modes
    this->tft.fillScreen(_DISPLAY_BACKGROUND_COLOR);
    this->_force_next_update = true;
}

Display::Mode Display::getMode() {
    return this->_mode;
}