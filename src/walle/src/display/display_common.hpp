#ifndef DISPLAY_COMMON_H
#define DISPLAY_COMMON_H

#include <TFT_eSPI.h> // Include the graphics library

#define _DISPLAY_WIDTH  320
#define _DISPLAY_HEIGHT 482

#define _DISPLAY_MARGIN 10
// The margin from the top of the display to the start of the GFXs. Used to seperate text and GFX
#define _DISPLAY_GFX_MARGIN 90

#define _DISPLAY_COLOR            TFT_YELLOW
#define _DISPLAY_BACKGROUND_COLOR TFT_BLACK


/********** Sun Defines **********/
#define _DISPLAY_SUN_INNER_DIAMETER 50
#define _DISPLAY_SUN_OUTER_DIAMETER 90
#define _DISPLAY_SUN_LINE_WIDTH     10
#define _DISPLAY_SUN_SPACING        10
#define _DISPLAY_SUN_NUM_RAYS       12


#endif