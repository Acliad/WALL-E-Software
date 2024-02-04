#ifndef CONTEXTS_HPP
#define CONTEXTS_HPP

#include "src/motion/servo_motor.hpp"

class ServoContext{
    public:
        ServoMotor* servo_neck_yaw;
        ServoMotor* servo_neck_pitch;
        ServoMotor* servo_eye_left;
        ServoMotor* servo_eye_right;
        ServoMotor* servo_shoulder_left;
        ServoMotor* servo_shoulder_right;
        ServoMotor* servo_elbow_left;
        ServoMotor* servo_elbow_right;
        ServoMotor* servo_wrist_left;
        ServoMotor* servo_wrist_right;
        ServoMotor* servo_hand_left;
        ServoMotor* servo_hand_right;
};

#endif // CONTEXTS_HPP
