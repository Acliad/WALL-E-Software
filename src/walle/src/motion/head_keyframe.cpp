#include "head_keyframe.hpp"

HeadKeyframe::HeadKeyframe(unsigned long duration_ms)
    : _next(nullptr), _prev(nullptr), _head(nullptr), _duration_ms(duration_ms) {
}

HeadKeyframe::~HeadKeyframe() {
    // Iterate through the keyframe's servo keyframes and delete them
    servo_keyframe *current = _head;
    while (current != nullptr) {
        servo_keyframe *next = current->_next;
        delete current;
        current = next;
    }
}

void HeadKeyframe::add_servo_angle(ServoMotor *servo, float angle, ramp_mode ramp_mode) {
    // Convert angle to a scalar and add the keyframe
    add_servo_scalar(servo, servo->angle_to_scalar(angle), ramp_mode);
}

void HeadKeyframe::add_servo_scalar(ServoMotor *servo, float scalar, ramp_mode ramp_mode) {
    // Create the servo keyframe element and add it to the linked list
    servo_keyframe *new_servo_keyframe = new servo_keyframe;
    new_servo_keyframe->_servo = servo;
    new_servo_keyframe->_target_scalar = scalar;
    new_servo_keyframe->_ramp_mode = ramp_mode;
    new_servo_keyframe->_next = nullptr;

    // Find the end of the linked list and add the new keyframe
    servo_keyframe *current_servo = _head;
    if (current_servo == nullptr) {
        _head = new_servo_keyframe;
    } else {
        while (current_servo->_next != nullptr) {
            current_servo = current_servo->_next;
        }
        current_servo->_next = new_servo_keyframe;
    }
}

void HeadKeyframe::start_keyframe() {
    // Iterate through the keyframe's servo keyframes and start them
    servo_keyframe *current = _head;
    while (current != nullptr) {
        // Set the ramp mode for this servo
        current->_servo->set_ramp_mode(current->_ramp_mode);
        // Set the value to ramp to for this servo
        current->_servo->set_scalar(current->_target_scalar, _duration_ms);

        current = current->_next;
    }
}

void HeadKeyframe::update_keyframe() {
    // Iterate through the keyframe's servo keyframes and update them
    servo_keyframe *current = _head;
    while (current != nullptr) {
        current->_servo->update();
        current = current->_next;
    }
}

void HeadKeyframe::set_next(HeadKeyframe *next) {
    // Set the next keyframe
    _next = next;
}

void HeadKeyframe::set_prev(HeadKeyframe *prev) {
    // Set the previous keyframe
    _prev = prev;
}

unsigned long HeadKeyframe::get_duration_ms() {
    return _duration_ms;
}

HeadKeyframe *HeadKeyframe::get_next() {
    return _next;
}

HeadKeyframe *HeadKeyframe::get_prev() {
    return _prev;
}
