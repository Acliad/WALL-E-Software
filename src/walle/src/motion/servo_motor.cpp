#include "servo_motor.hpp"

ServoMotor::ServoMotor(Adafruit_PWMServoDriver *pca9685, int pin, int min_us, int max_us, int min_angle_deg,
                       int max_angle_deg) {}


void Motor::set_angle(float angle) {
    // Convert angle to the closest us value and call set_us. Angle is automatically constrained to the min and max.

    // Constraint the angle to the min and max
    angle = constrain(angle, _min_angle_deg, _max_angle_deg);
    int us = _angle_to_us_intercept + (_angle_to_us_slope * angle);

    this->set_us(us);
}


float Motor::get_angle() { return this->us_to_angle(_current_us); }


float Motor::us_to_angle(int us) {
    // Convert the given us to an angle
    float angle = (us - _angle_to_us_intercept) / _angle_to_us_slope;
    return angle;
}