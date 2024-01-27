/**
 * @file display.hpp
 * @author Isaac Rex (@Acliad)
 * @brief Class for controlling the solar display on a WALL-E robot. This class is responsible for drawing the solar 
 *        display and updating it. It also provides an interface for setting the status of the solar bars and animating 
 *        them with keyframes. 
 * @version 0.1
 * @date 2023-09-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Arduino.h>
#include <TFT_eSPI.h>

#include "solar_bars.hpp"
#include "animate_level.hpp"
#include "HelveticaBoldSolar29.h"

/********** Font Defines **********/
#define _DISPLAY_FONT_SIZE 4
#define _DISPLAY_FONT_NAME HelveticaBoldSolar29

/**
 * @brief Class for controlling the solar display on a WALL-E robot. This class is responsible for drawing the solar 
 *        display and updating it. It also provides an interface for setting the status of the solar bars and animating 
 *        them with keyframes. 
 */
class SolarDisplay {
public:
    SolarDisplay(TFT_eSPI& tft);

    /**
     * @brief Initialize the display for use
     * 
     */
    void begin();

    /**
     * @brief Set the status of a bar
     * 
     * @param index The index of the bar to set
     * @param on ‘true’ if the bar should be on, ‘false’ if it should be off
     */
    void setBar(unsigned int index, bool on);
    bool isBarOn(unsigned int index);

    /**
     * @brief Update the solar display. This should be called at least once per loop to update animations. 
     * 
     */
    void update();

    /**
     * @brief Set the animation object
     * 
     * @param animation The animation to set
     */
    void setAnimation(AnimateLevel animation);
    AnimateLevel getAnimation();

    /**
     * @brief Start the animation
     * 
     */
    void startAnimation();

    /**
     * @brief Stop the animation
     * 
     */
    void stopAnimation();

    /**
     * @brief Return the status of the animation
     * 
     * @return true if the animation is running, false otherwise
     */
    bool isAnimationRunning();

private:

    TFT_eSPI& tft;
    SolarBars solar_bars;
    AnimateLevel animation;

    /**
     * @brief Initialize the display appearance by drawing the sun and SOLAR CHARGE LEVEL text
     * 
     */
    void _draw_static_display_elements();
};

#endif