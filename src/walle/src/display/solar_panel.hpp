#ifndef BARS_H
#define BARS_H

#include <Arduino.h>
#include "display_common.hpp"
#include "EurostileBold31.h"

#define _SOLAR_PANEL_NUM_BARS 10

#define _BARS_LARGE_RECT_WIDTH 175
#define _BARS_LARGE_RECT_HEIGHT 50

#define _BARS_SMALL_RECT_WIDTH _BARS_LARGE_RECT_WIDTH
#define _BARS_SMALL_RECT_HEIGHT 17

#define _BARS_CORNER_RADIUS 2
#define _BARS_RECT_SPACING 18

#define _SOLAR_FONT_NAME EurostileBold31


class SolarPanel{
public:
    SolarPanel();

    void setBar(unsigned int index, bool on);
    void setAllBars(bool* bar_status);
    bool isBarOn(unsigned int index);

    void setSun(bool on);
    bool isSunOn();

    // Draw the solar panel on the display
    void drawOn(TFT_eSPI &tft);
    // Force update the solar panel on the display. Redraws all elements regardless of state
    void drawOnForce(TFT_eSPI &tft);

  private:
    struct SolarPanelState {
        bool bar_status[_SOLAR_PANEL_NUM_BARS];
        bool sun_on;
    };
    // Track the desired state of the display, to be updated with drawOn()
    SolarPanelState _set_state;
    // Track the current state of the display
    SolarPanelState _current_state;

    void _drawSun(TFT_eSPI &tft);
    void _blankSun(TFT_eSPI &tft);
    void _drawBars(TFT_eSPI &tft, bool force_redraw);
};

#endif