#include "servo_motor.hpp"

ServoMotor::ServoMotor(Adafruit_PWMServoDriver *pca9685, int pin, int neutral_us, int min_us, int max_us,
                       float min_angle_deg, float max_angle_deg, float neutral_angle_deg)
    : Motor(pca9685, pin, neutral_us, min_us, max_us), _min_angle_deg(min_angle_deg), _max_angle_deg(max_angle_deg),
      _neutral_angle_deg(neutral_angle_deg) {
    // Calculate the slope and intercept for the angle to us mapping
    _angle_positive_to_us_slope = (float)(_max_us - _neutral_us) / (_max_angle_deg - _neutral_angle_deg);
    _angle_negative_to_us_slope = (float)(_neutral_us - _min_us) / (_neutral_angle_deg - _min_angle_deg);
    _target_us = _neutral_us;
    _us_ramp = rampInt();
    _ramp_mode = QUADRATIC_INOUT;
}

void  ServoMotor::set_min_us(int min_us) {
    // Set minimum us, recalculate slope for asymetric mapping
    Motor::set_min_us(min_us);
    _angle_negative_to_us_slope = (float)(_neutral_us - _min_us) / (_neutral_angle_deg - _min_angle_deg);
}
void  ServoMotor::set_max_us(int max_us) {
    // Update max us and its slope
    Motor::set_max_us(max_us);
    _angle_positive_to_us_slope = (float)(_max_us - _neutral_us) / (_max_angle_deg - _neutral_angle_deg);
}
void  ServoMotor::set_neutral_us(int neutral_us) {
    // Update neutral us and slopes
    Motor::set_neutral_us(neutral_us);
    _angle_positive_to_us_slope = (float)(_max_us - _neutral_us) / (_max_angle_deg - _neutral_angle_deg);
    _angle_negative_to_us_slope = (float)(_neutral_us - _min_us) / (_neutral_angle_deg - _min_angle_deg);
}

void ServoMotor::set_angle(float angle, unsigned long time_ms) {
    // Convert target angle to target us, then set up the ramp
    _target_us = angle_to_us(angle);
    _us_ramp.go(_target_us, time_ms, _ramp_mode);
}

void ServoMotor::set_min_angle(float min_angle_deg) {
    // Set minimum angle, recalculate slope for asymetric mapping
    _min_angle_deg = min_angle_deg;
    _angle_negative_to_us_slope = (float)(_neutral_us - _min_us) / (_neutral_angle_deg - _min_angle_deg);
}

void ServoMotor::set_max_angle(float max_angle_deg) {
    // Update max angle and its slope
    _max_angle_deg = max_angle_deg;
    _angle_positive_to_us_slope = (float)(_max_us - _neutral_us) / (_max_angle_deg - _neutral_angle_deg);

}

void ServoMotor::set_neutral_angle(float neutral_angle_deg) {
    // Update neutral angle and slopes
    _neutral_angle_deg = neutral_angle_deg;
    _angle_positive_to_us_slope = (float)(_max_us - _neutral_us) / (_max_angle_deg - _neutral_angle_deg);
    _angle_negative_to_us_slope = (float)(_neutral_us - _min_us) / (_neutral_angle_deg - _min_angle_deg);
}

void ServoMotor::set_scalar(float scalar, unsigned long time_ms) {
    // Set the speed as a scaller from -1.0 to 1.0. -1.0 is min_us, 1.0 is max_us.
    _target_us = _scalar_to_us(scalar);
    _us_ramp.go(_target_us, time_ms, _ramp_mode);
}

void ServoMotor::set_ramp_mode(ramp_mode mode) {
    _ramp_mode = mode;
}

void ServoMotor::update() {
    // Update the ramp and set the motor to the current us
    _current_us = _us_ramp.update();
    Motor::update();
}

float ServoMotor::get_angle() {
    return us_to_angle(_current_us);
}

float ServoMotor::angle_to_scalar(float angle_deg) {
    // Convert the given angle to the corresponding scalar, accounting for asymetric mapping
    if (angle_deg > _neutral_angle_deg) {
        return (float)(angle_deg - _neutral_angle_deg) / (_max_angle_deg - _neutral_angle_deg);
    } else {
        return (float)(angle_deg - _neutral_angle_deg) / (_neutral_angle_deg - _min_angle_deg);
    }
}

float ServoMotor::us_to_angle(int us) {
    // Returns the closest the angle corresponding to the given us
    if (us > _neutral_us) {
        return (float)(us - _neutral_us) / _angle_positive_to_us_slope + _neutral_angle_deg;
    } else {
        return (float)(us - _neutral_us) / _angle_negative_to_us_slope + _neutral_angle_deg;
    }
}

float ServoMotor::angle_to_us(float angle_deg) {
    // Converts the given angle to the corresponding us, accounting for asymetric mapping
    if (angle_deg > _neutral_angle_deg) {
        return (float)(angle_deg - _neutral_angle_deg) * _angle_positive_to_us_slope + _neutral_us;
    } else {
        return (float)(angle_deg - _neutral_angle_deg) * _angle_negative_to_us_slope + _neutral_us;
    }
}
