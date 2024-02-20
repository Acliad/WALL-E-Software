/**
 * @file display_common.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains common defines for the display module. This file is included in all display classes to
 * provide a common set of defines for the display module.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef DISPLAY_COMMON_H
#define DISPLAY_COMMON_H

#include <TFT_eSPI.h> // Include the graphics library

#define _DISPLAY_WIDTH  TFT_WIDTH
#define _DISPLAY_HEIGHT TFT_HEIGHT

// TODO: Maybe these should live in the solar_panel.hpp file?
#define _DISPLAY_COLOR            TFT_YELLOW
#define _DISPLAY_BACKGROUND_COLOR TFT_BLACK

#define _DISPLAY_MARGIN 10
// The margin from the top of the display to the start of the GFXs. Used to seperate text and GFX
#define _DISPLAY_GFX_MARGIN 90


#endif