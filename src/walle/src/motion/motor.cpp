/**
 * @file motor.cpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the implementation of the Motor class, which is used to control a motor using a PCA9685
 * PWM driver.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "motor.hpp"

Motor::Motor(Adafruit_PWMServoDriver *pca9685, int pin, std::string name, int neutral_us, int min_us, int max_us)
    : _pwm_driver(pca9685), _pin(pin), _neutral_us(neutral_us), _min_us(min_us), _max_us(max_us), _name(name) {
    // Calculate the slope and intercept for the scalar to us mapping
    _scalar_plus_to_us_slope = (float)(_max_us - _neutral_us) / 1.0f;
    _scalar_minus_to_us_slope = (float)(_neutral_us - _min_us) / 1.0f;

    // Set the motor to neutral
    _current_us = _neutral_us;
    _last_update_ms = 0;
}

void Motor::update() {
    // write _current_us to _pin

    // NOTE: Adafruit's microseconds function reads the prescaler every time this is called, which is a bit unnecessary.
    // If there becomes a reason to, we can directly callculate the OCR value (4095 counter) from us and our best guess
    // at clock rate. Servos don't seem to be too picky.
    _pwm_driver->writeMicroseconds(_pin, _current_us);
}

void Motor::set_us(int us) {
    _current_us = constrain(us, _min_us, _max_us);
}

void Motor::set_scalar(float scalar) {
    // Set the scalar as a scaller from -1.0 to 1.0. -1.0 is min_us, 1.0 is max_us.
    _current_us = _scalar_to_us(scalar);
}

void Motor::set_min_us(int min_us) {
    _min_us = min_us;
    // Recalculate the slope and intercept for the scalar to us mapping
    _scalar_minus_to_us_slope = (float)(_neutral_us - _min_us) / 1.0f;
}

void Motor::set_max_us(int max_us) {
    _max_us = max_us;
    // Recalculate the slope and intercept for the scalar to us mapping
    _scalar_plus_to_us_slope = (float)(_max_us - _neutral_us) / 1.0f;
}

void Motor::set_neutral_us(int neutral_us) {
    _neutral_us = neutral_us;
    // Recalculate the slope and intercept for the scalar to us mapping
    _scalar_plus_to_us_slope = (float)(_max_us - _neutral_us) / 1.0f;
    _scalar_minus_to_us_slope = (float)(_neutral_us - _min_us) / 1.0f;
}

int Motor::get_current_us() {
    return _current_us;
}

float Motor::get_current_scalar() {
    return _us_to_scaler(_current_us);
}

std::string Motor::get_name() {
    return _name;
}

int Motor::_scalar_to_us(float scalar) {
    // Return the closest us to the given scalar, account for asymetric mapping of _min_us and _max_us.
    if (scalar > 0) {
        return (int)(scalar * _scalar_plus_to_us_slope + _neutral_us);
    } else {
        return (int)(scalar * _scalar_minus_to_us_slope + _neutral_us);
    }
}

float Motor::_us_to_scaler(int us) {
    // Calculates current scalar from us, account for asymetric mapping of _min_us and _max_us.
    if (us > _neutral_us) {
        return (float)(us - _neutral_us) / _scalar_plus_to_us_slope;
    } else {
        return (float)(us - _neutral_us) / _scalar_minus_to_us_slope;
    }
}
