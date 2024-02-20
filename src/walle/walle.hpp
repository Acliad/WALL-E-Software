/**
 * @file walle.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the main include file for the Walle project. This file includes all other files in the
 * project, and is used to provide a single include file for the main entry sketch.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef WALLE_H
#define WALLE_H

#include "config.hpp"
#include "src/controller/navigation_controller.hpp"
#include "src/motion/drive_motor.hpp"
#include "src/motion/servo_motor.hpp"
#include "src/motion/animate_servo_recorder.hpp"
#include "src/display/display.hpp"
#include "src/button/button.hpp"
#include "src/stats/stats.hpp"
#include "src/motion/servo_player.hpp"
#include "display_animations.hpp"
#include "motion_animations.hpp"
#include "src/motion/servo_context.hpp"
#include "src/audio/audio_player.hpp"
#include <Adafruit_PWMServoDriver.h>
#include <Bluepad32.h>
#include <Wire.h>
#include <SPIFFS.h>

/**************************************************************
 *                           Macros                           *
 **************************************************************/
#define ARRAY_SIZE(x) static_cast<int>(sizeof(x) / sizeof(x[0]))

/**************************************************************
 *                          Defines                           *
 **************************************************************/
#define PI_OVER_FOUR (PI / 4.0)

#endif // WALLE_H