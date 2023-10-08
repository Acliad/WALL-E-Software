#include "motor.hpp"

Motor::Motor(Adafruit_PWMServoDriver *pca9685, int pin, int min_us, int max_us, int min_angle_deg, int max_angle_deg, float max_speed)
    : _pwm_driver(pca9685), _pin(pin), _min_us(min_us), _max_us(max_us), _neutral_us((max_us + min_us) / 2),
      _current_us(_neutral_us), _min_angle_deg(min_angle_deg), _max_angle_deg(max_angle_deg), _max_speed(1.0) {

    // Calculate the slope and intercept for converting angles to us
    // Do the full y = mx + b since angle could be asymetric
    _angle_to_us_slope = (_max_us - _min_us) / (_max_angle_deg - _min_angle_deg);
    _angle_to_us_intercept = _max_us - (_angle_to_us_slope * _max_angle_deg);
}

// void Motor::begin() {}

void Motor::set_us(int us) {
    // Set the current desired us. Note that this does not write to the pwm_driver. Call update() to do that.

    // Constraint the us to the min and max
    _current_us = constrain(us, _min_us, _max_us);
}

void Motor::set_speed(float speed) {
    // Constrain speed between -1.0 and 1.0
    speed = constrain(speed, -1.0f, 1.0f) * _max_speed;
    // Converts speed to the closest us value and calls set_us
    int us = _neutral_us + (speed * (_max_us - _min_us) / 2);
    this->set_us(us);
}

void Motor::set_speed_limit(float max_speed) {
    // constrain the max speed to the min and max
    _max_speed = constrain(max_speed, -1.0f, 1.0f);
}
void Motor::set_angle(float angle) {
    // Convert angle to the closest us value and call set_us. Angle is automatically constrained to the min and max.

    // Constraint the angle to the min and max
    angle = constrain(angle, _min_angle_deg, _max_angle_deg);
    int us = _angle_to_us_intercept + (_angle_to_us_slope * angle);

    this->set_us(us);
}

int Motor::get_us() { return _current_us; }

float Motor::get_speed() { return this->us_to_speed(_current_us); }

float Motor::get_angle() { return this->us_to_angle(_current_us); }

float Motor::us_to_speed(int us) {
    // Convert the given us to a speed

    // TODO: Test
    float speed = (us - _neutral_us) / ((_max_us - _min_us) / 2);
    return speed;
}

float Motor::us_to_angle(int us) {
    // Convert the given us to an angle
    float angle = (us - _angle_to_us_intercept) / _angle_to_us_slope;
    return angle;
}

float Motor::update() {
    // Set the pwm_driver's output to the give us

    // NOTE: Adafruit's microseconds function reads the prescaler every time this is called, which is a bit unnessisary.
    // If there becomes a reason to, we can directly callculate the OCR value (4095 counter) from us and our best guess
    // at clock rate. Servos don't seem to be too picky

    _pwm_driver->writeMicroseconds(_pin, _current_us);
    return this->get_speed();
}
