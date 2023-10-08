#include "motion.hpp"

// Create skelltons for the header file's functions
Motion::Motion(Motor *motor, ramp_mode ramp_mode, float acceleration_per_ss, float deceleration_per_ss, float max_speed,
               float min_speed)
    : _motor(motor), _ramp_mode(ramp_mode), _acceleration_per_ms(acceleration_per_ss / 1000.0f),
      _deceleration_per_ms(deceleration_per_ss / 1000.0f), _current_speed(_motor->get_speed()), _max_speed(max_speed) {}

void Motion::begin() { _ramp = rampFloat(_current_speed); }

void Motion::set_speed(float speed) {
    // Set speed limits
    speed = constrain(speed, -1.0f, 1.0f);
    _current_speed = speed * _max_speed;
}
void Motion::update() {
    // Update the motor's speed
    unsigned long dt_ms = millis() - last_update_time_ms;

    // NOTE: If we add more acceleration types, we can add a switch statement here:
    float new_speed = _get_speed_constant_accel(dt_ms);

    _motor->set_speed(new_speed);
}

void Motion::set_ramp_mode(ramp_mode ramp_mode) { _ramp_mode = ramp_mode; }

void Motion::set_acceleration(float acceleration_per_ss) { _acceleration_per_ms = acceleration_per_ss / 1000.0f; }

void Motion::set_deceleration(float deceleration_per_ss) { _deceleration_per_ms = deceleration_per_ss / 1000.0f; }

void Motion::set_speed_limit(float max_speed) {
    // constrain the max speed to the min and max
    _max_speed = constrain(max_speed, -1.0f, 1.0f);
}

float Motion::_get_speed_constant_accel(unsigned long dt_ms) {
    // Returns what the updated speed should be for the give dt_ms for a constant acceleraion curve (linear velocity
    // change)
    // Check if we're already at the target speed
}
