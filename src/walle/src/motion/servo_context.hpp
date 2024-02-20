/**
 * @file servo_context.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file defines the ServoContext class, which is used to store and manage servo motor instances across the
 * program. It also contains defines for the names of the physical servos. 
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef SEREVO_CONTEXT_HPP
#define SEREVO_CONTEXT_HPP

#include "servo_motor.hpp"
#include <map>

#define MOTOR_LEFT_NAME           "Left Track"
#define MOTOR_RIGHT_NAME          "Right Track"
#define SERVO_EYE_LEFT_NAME       "Left Eye"
#define SERVO_EYE_RIGHT_NAME      "Right Eye"
#define SERVO_NECK_PITCH_NAME     "Neck Pitch"
#define SERVO_NECK_YAW_NAME       "Neck Yaw"
#define SERVO_SHOULDER_LEFT_NAME  "Left Shoulder"
#define SERVO_SHOULDER_RIGHT_NAME "Right Shoulder"
#define SERVO_ELBOW_LEFT_NAME     "Left Elbow"
#define SERVO_ELBOW_RIGHT_NAME    "Right Elbow"
#define SERVO_WRIST_LEFT_NAME     "Left Wrist"
#define SERVO_WRIST_RIGHT_NAME    "Right Wrist"
#define SERVO_HAND_LEFT_NAME      "Left Hand"
#define SERVO_HAND_RIGHT_NAME     "Right Hand"

/**
 * @brief The ServoContext class represents a context for servo motors.
 * 
 * TODO: This should be converted to a singleton class
 * 
 * This class provides a static map that associates servo motor names with their corresponding ServoMotor objects.
 * It allows easy access to servo motors by name throughout the application.
 */
class ServoContext{
    public:
        std::map <std::string, ServoMotor*> map;
};

#endif // SEREVO_CONTEXT_HPP
