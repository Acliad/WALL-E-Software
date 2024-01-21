#include "solar_bars.hpp"

SolarBars::SolarBars(TFT_eSPI& tft) : bar_status(), tft(tft) {

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

        tft.fillRect(_DISPLAY_WIDTH - _DISPLAY_MARGIN - rect_width, 
                     ypos, 
                     rect_width, 
                     rect_height, 
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
