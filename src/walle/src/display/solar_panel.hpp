/**
 * @file solar_panel.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the declaration of the SolarPanel class, which is used to control the display of the
 * solar panel page.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef BARS_H
#define BARS_H

#include <Arduino.h>
#include "display_common.hpp"
#include "EurostileBold31.h"

/*----------- Solar Bars --------------------------------*/
#define _SOLAR_PANEL_NUM_BARS 10

#define _BARS_LARGE_RECT_WIDTH 175
#define _BARS_LARGE_RECT_HEIGHT 50

#define _BARS_SMALL_RECT_WIDTH _BARS_LARGE_RECT_WIDTH
#define _BARS_SMALL_RECT_HEIGHT 17

#define _BARS_CORNER_RADIUS 2
#define _BARS_RECT_SPACING 18

/*----------- Sun ---------------------------------------*/
#define _DISPLAY_SUN_INNER_DIAMETER 50
#define _DISPLAY_SUN_OUTER_DIAMETER 90
#define _DISPLAY_SUN_LINE_WIDTH     10
#define _DISPLAY_SUN_SPACING        10
#define _DISPLAY_SUN_NUM_RAYS       12

/*----------- Font --------------------------------------*/
#define _SOLAR_FONT_NAME EurostileBold31

/**
 * @class SolarPanel
 * @brief Class representing a solar panel display.
 * 
 * This class provides methods to control and draw a solar panel on a display.
 * It allows setting the status of individual bars and the sun, as well as drawing
 * the solar panel on the display.
 */
class SolarPanel{
public:
  /**
   * @brief Default constructor for the SolarPanel class.
   */
  SolarPanel();

  /**
   * @brief Set the status of a specific bar on the solar panel.
   * 
   * @param index The index of the bar to set.
   * @param on The status of the bar (true for on, false for off).
   */
  void setBar(unsigned int index, bool on);

  /**
   * @brief Set the status of all bars on the solar panel.
   * 
   * @param bar_status An array of boolean values representing the status of each bar.
   *                   The size of the array should be equal to _SOLAR_PANEL_NUM_BARS.
   */
  void setAllBars(bool bar_status[_SOLAR_PANEL_NUM_BARS]);

  /**
   * @brief Check if a specific bar on the solar panel is on.
   * 
   * @param index The index of the bar to check.
   * @return true if the bar is on, false otherwise.
   */
  bool isBarOn(unsigned int index);

  /**
   * @brief Set the status of the sun on the solar panel.
   * 
   * @param on The status of the sun (true for on, false for off).
   */
  void setSun(bool on);

  /**
   * @brief Check if the sun on the solar panel is on.
   * 
   * @return true if the sun is on, false otherwise.
   */
  bool isSunOn();

  /**
   * @brief Draw the solar panel on the display.
   * 
   * @param tft The TFT_eSPI object representing the display.
   */
  void drawOn(TFT_eSPI &tft);

  /**
   * @brief Force update the solar panel on the display.
   * 
   * This method redraws all elements of the solar panel on the display,
   * regardless of their current state.
   * 
   * @param tft The TFT_eSPI object representing the display.
   */
  void drawOnForce(TFT_eSPI &tft);

private:
  /**
   * @brief Struct representing the state of the solar panel.
   */
  struct SolarPanelState {
    bool bar_status[_SOLAR_PANEL_NUM_BARS]; // Array representing the status of each bar
    bool sun_on; // Status of the sun
  };

  SolarPanelState _set_state;     // Desired state of the display
  SolarPanelState _current_state; // Current state of the display

  /**
   * @brief Draw the sun on the display.
   * 
   * @param tft The TFT_eSPI object representing the display.
   */
  void _drawSun(TFT_eSPI &tft);

  /**
   * @brief Blank out the sun on the display.
   * 
   * @param tft The TFT_eSPI object representing the display.
   */
  void _blankSun(TFT_eSPI &tft);

  /**
   * @brief Draw the bars on the display.
   * 
   * @param tft The TFT_eSPI object representing the display.
   * @param force_redraw Flag indicating whether to force a redraw of all bars.
   */
  void _drawBars(TFT_eSPI &tft, bool force_redraw);
};

#endif