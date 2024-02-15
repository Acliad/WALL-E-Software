#ifndef CONTEXTS_HPP
#define CONTEXTS_HPP

#include "servo_motor.hpp"
#include <vector>

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

        void applyToAllServos(std::function<void(ServoMotor*)> func) {
            func(servo_neck_yaw);
            func(servo_neck_pitch);
            func(servo_eye_left);
            func(servo_eye_right);
            func(servo_shoulder_left);
            func(servo_shoulder_right);
            func(servo_elbow_left);
            func(servo_elbow_right);
            func(servo_wrist_left);
            func(servo_wrist_right);
            func(servo_hand_left);
            func(servo_hand_right);
        }

        ServoMotor* getByName(std::string name) {
            // TODO: This is a little bit of a code smell. The entire servo context interface needs redone.
            if (name == SERVO_NECK_YAW_NAME) {
                return servo_neck_yaw;
            } else if (name == SERVO_NECK_PITCH_NAME) {
                return servo_neck_pitch;
            } else if (name == SERVO_EYE_LEFT_NAME) {
                return servo_eye_left;
            } else if (name == SERVO_EYE_RIGHT_NAME) {
                return servo_eye_right;
            } else if (name == SERVO_SHOULDER_LEFT_NAME) {
                return servo_shoulder_left;
            } else if (name == SERVO_SHOULDER_RIGHT_NAME) {
                return servo_shoulder_right;
            } else if (name == SERVO_ELBOW_LEFT_NAME) {
                return servo_elbow_left;
            } else if (name == SERVO_ELBOW_RIGHT_NAME) {
                return servo_elbow_right;
            } else if (name == SERVO_WRIST_LEFT_NAME) {
                return servo_wrist_left;
            } else if (name == SERVO_WRIST_RIGHT_NAME) {
                return servo_wrist_right;
            } else if (name == SERVO_HAND_LEFT_NAME) {
                return servo_hand_left;
            } else if (name == SERVO_HAND_RIGHT_NAME) {
                return servo_hand_right;
            } else {
                return nullptr;
            }
        }
};

#endif // CONTEXTS_HPP
