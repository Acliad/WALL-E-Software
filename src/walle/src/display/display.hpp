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

#include "display_common.hpp"
#include "solar_panel.hpp"
#include "recording_panel.hpp"
#include "animate_solar_panel.hpp"

/**
 * @brief Class for controlling the solar display on a WALL-E robot. This class is responsible for drawing the solar 
 *        display and updating it. It also provides an interface for setting the status of the solar bars and animating 
 *        them with keyframes. 
 */
class Display {
public:
    enum class Mode {
        SOLAR_PANEL,
        RECORDER
    };

    SolarPanel solar_panel;
    RecordingPanel recording_panel;
    
    Display(TFT_eSPI& tft);

    /**
     * @brief Initialize the display for use
     * 
     */
    void begin();

    /**
     * @brief Sets the mode of the display.
     *
     * @param mode The mode to set.
     */
    void setMode(Mode mode);

    /**
     * @brief Get the mode of the display.
     *
     * @return The mode of the display.
     */
    Mode getMode();

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
    void setAnimation(AnimateSolarPanel animation);
    AnimateSolarPanel getAnimation();

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
    AnimateSolarPanel animation;
    Mode _mode;
    bool _force_next_update;

    /**
     * @brief Initialize the display appearance by drawing the sun and SOLAR CHARGE LEVEL text
     * 
     */
    void _draw_static_display_elements();
};

#endif