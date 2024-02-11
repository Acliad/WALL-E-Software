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
#include <Adafruit_PWMServoDriver.h>
#include <Bluepad32.h>
#include <DFMiniMp3.h>
#include <Wire.h>

/**************************************************************
 *                           Macros                           *
 **************************************************************/
#define ARRAY_SIZE(x) static_cast<int>(sizeof(x) / sizeof(x[0]))

/**************************************************************
 *                          Defines                           *
 **************************************************************/
#define PI_OVER_FOUR (PI / 4.0)


#endif // WALLE_H