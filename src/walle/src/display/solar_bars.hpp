#ifndef BARS_H
#define BARS_H

#include <Arduino.h>
#include "display_common.hpp"

#define _BARS_NUM_BARS 15

#define _BARS_LARGE_RECT_WIDTH 150
#define _BARS_LARGE_RECT_HEIGHT 45

#define _BARS_SMALL_RECT_WIDTH _BARS_LARGE_RECT_WIDTH
#define _BARS_SMALL_RECT_HEIGHT 18
#define _BARS_RECT_SPACING 6

class SolarBars{
public:
    SolarBars(TFT_eSPI& tft);

    void begin();

    void setBar(unsigned int index, bool on);
    void setAllBars(bool* bar_status);
    bool isBarOn(unsigned int index);

private:
    TFT_eSPI& tft;
    bool bar_status[_BARS_NUM_BARS];
};

#endif