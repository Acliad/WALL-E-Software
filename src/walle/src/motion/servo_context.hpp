#ifndef CONTEXTS_HPP
#define CONTEXTS_HPP

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

class ServoContext{
    public:
        std::map <std::string, ServoMotor*> map;
};

#endif // CONTEXTS_HPP
