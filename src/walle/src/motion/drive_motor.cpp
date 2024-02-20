/**
 * @file drive_motor.cpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the implementation of the DriveMotor class, which is used to control a track motor.
 * @version 0.1
 * @date 2024-02-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "drive_motor.hpp"

DriveMotor::DriveMotor(Adafruit_PWMServoDriver *pca9685, int pin, int min_us, int max_us)
    : _pwm_driver(pca9685), _pin(pin), _min_us(min_us), _max_us(max_us), _neutral_us((max_us + min_us) / 2),
      _max_speed(1.0), _acceleration_per_ms(_DRIVE_MOTOR_DEFAULT_ACCELERATION),
      _deceleration_per_ms(_DRIVE_MOTOR_DEFAULT_ACCELERATION), _last_update_ms(0) {}

void DriveMotor::set_speed(float speed) {
    // Constrain speed between -1.0 and 1.0
    speed = constrain(speed, -1.0f, 1.0f);
    // Map the given speed acording to the max speed
    _target_speed = speed * _max_speed;
}

void DriveMotor::set_speed_limit(float max_speed) {
    // constrain the max speed to the min and max
    _max_speed = constrain(max_speed, -1.0f, 1.0f);
    // Check if the current target speed is greater than the new max speed and update it if necessary
    if (_target_speed > _max_speed) {
        _target_speed = _max_speed;
    } else if (_target_speed < -_max_speed) {
        _target_speed = -_max_speed;
    }
}

void DriveMotor::set_acceleration(float acceleration_per_ss) {
    // Store acceleration per ms^2 instead of per s^2 so we don't have to do conversions every time we update
    _acceleration_per_ms = acceleration_per_ss / _DRIVE_MOTOR_S_TO_MS;
}

void DriveMotor::set_deceleration(float deceleration_per_ss) {
    // Store deceleration per ms^2 instead of per s^2 so we don't have to do conversions every time we update. Pass as a
    // positive value

    _deceleration_per_ms = deceleration_per_ss / _DRIVE_MOTOR_S_TO_MS;
}

float DriveMotor::update() {
    // Set the pwm_driver's output to the give us

    // NOTE: Adafruit's microseconds function reads the prescaler every time this is called, which is a bit unnessisary.
    // If there becomes a reason to, we can directly callculate the OCR value (4095 counter) from us and our best guess
    // at clock rate. Servos don't seem to be too picky

    // If we add more acceleration types, we can add a switch here
    this->_update_speed_constant_accel();

    _pwm_driver->writeMicroseconds(_pin, _speed_to_us(_current_speed));
    return this->get_current_speed();
}

float DriveMotor::get_current_speed() { return _current_speed; }

float DriveMotor::get_speed_limit() { return _max_speed; }

void DriveMotor::_update_speed_constant_accel() {
    // This is a bit tricky because there are multiple cases that have to be handeld differently:
    //
    //     - _current_speed is positive and we're accelerating toward 1
    //     - _current_speed is positive and we're decelerating toward 0
    //     - _current_speed is negative and we're accelerating toward -1
    //     - _current_speed is negative and we're decelerating toward 0
    //     - _current_speed is 0 and we're accelerating toward 1
    //     - _current_speed is 0 and we're accelerating toward -1
    //
    // Perhaps there's a super clever way to roll this into a clean, succinct function, but I can't think of it.
    // So for now, we'll just do it the long way.

    // First check if we're already at the target speed. If so, don't do anything
    if (_current_speed == _target_speed) {
        return;
    }

    // Othewise, we're going to need dt
    unsigned long current_time_ms = millis();
    unsigned long dt_ms = current_time_ms - _last_update_ms;
    _last_update_ms = current_time_ms;

    if (_current_speed > 0) {
        if (_current_speed > _target_speed) {
            // _current_speed is positive and we're decelerating toward 0
            _current_speed = max(_current_speed - (_deceleration_per_ms * dt_ms), _target_speed);
        } else {
            // _current_speed is positive and we're accelerating toward 1
            _current_speed = min(_current_speed + (_acceleration_per_ms * dt_ms), _target_speed);
        }
    } else if (_current_speed < 0) {
        if (_current_speed < _target_speed) {
            // _current_speed is negative and we're decelerating toward 0
            _current_speed = min(_current_speed + (_deceleration_per_ms * dt_ms), _target_speed);
        } else {
            // _current_speed is negative and we're accelerating toward -1
            _current_speed = max(_current_speed - (_acceleration_per_ms * dt_ms), _target_speed);
        }
    } else { // Special case where _current_speed is 0
        if (_target_speed > 0) {
            // _current_speed is 0 and we're accelerating toward 1
            _current_speed = min(_current_speed + (_acceleration_per_ms * dt_ms), _target_speed);
        } else {
            // _current_speed is 0 and we're accelerating toward -1
            _current_speed = max(_current_speed - (_acceleration_per_ms * dt_ms), _target_speed);
        }
    }
}

unsigned int DriveMotor::_speed_to_us(float speed) {
    // Convert speed to the closest us value
    unsigned int us = _neutral_us + (speed * (_max_us - _min_us) / 2);
    return us;
}

float DriveMotor::_us_to_speed(int us) {
    // Convert the given us to a speed

    // TODO: Test
    float speed = (us - _neutral_us) / ((_max_us - _min_us) / 2);
    return speed;
}