#ifndef BARS_H
#define BARS_H

#include <Arduino.h>
#include "display_common.hpp"

#define _BARS_NUM_BARS 10

#define _BARS_LARGE_RECT_WIDTH 175
#define _BARS_LARGE_RECT_HEIGHT 50

#define _BARS_SMALL_RECT_WIDTH _BARS_LARGE_RECT_WIDTH
#define _BARS_SMALL_RECT_HEIGHT 17

#define _BARS_CORNER_RADIUS 3
#define _BARS_RECT_SPACING 18

class SolarBars{
public:
    SolarBars(TFT_eSPI& tft);

    void begin();

    void setBar(unsigned int index, bool on);
    void setAllBars(bool* bar_status);
    bool isBarOn(unsigned int index);

    void setSun(bool on);
    bool isSunOn();

private:
    TFT_eSPI& tft;
    bool bar_status[_BARS_NUM_BARS];
    bool sun_on;

    void _drawSun();
    void _blankSun();
};

#endif