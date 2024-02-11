#include "servo_keyframe.hpp"

ServoKeyframe::ServoKeyframe(unsigned long duration_ms)
    : _next(nullptr), _prev(nullptr), _servo_head(nullptr), _duration_ms(duration_ms) {
}

ServoKeyframe::ServoKeyframe(const ServoKeyframe &keyframe) {
    _next = nullptr;
    _prev = nullptr;
    _servo_head = nullptr;

    // Copy the duration
    _duration_ms = keyframe._duration_ms;

    // Copy the linked list of servo_node elements
    servo_node *current = keyframe._servo_head;
    while (current != nullptr) {
        add_servo_scalar(current->_servo, current->_target_scalar, current->_ramp_mode);
        current = current->_next;
    }
}

ServoKeyframe::~ServoKeyframe() {
    // Delete the linked list of servo_node elements
    servo_node *current = _servo_head;
    while (current != nullptr) {
        servo_node *next = current->_next;
        delete current;
        current = next;
    }
}

void ServoKeyframe::add_servo_angle(ServoMotor *servo, float angle, ramp_mode ramp_mode) {
    // Convert angle to a scalar and add the keyframe
    add_servo_scalar(servo, servo->angle_to_scalar(angle), ramp_mode);
}

void ServoKeyframe::add_servo_scalar(ServoMotor *servo, float scalar, ramp_mode ramp_mode) {
    // Create the servo keyframe element and add it to the linked list
    servo_node *new_servo_keyframe = new servo_node;
    new_servo_keyframe->_servo = servo;
    new_servo_keyframe->_target_scalar = scalar;
    new_servo_keyframe->_ramp_mode = ramp_mode;
    new_servo_keyframe->_next = nullptr;

    // Find the end of the linked list and add the new keyframe
    servo_node *current_servo = _servo_head;
    if (current_servo == nullptr) {
        _servo_head = new_servo_keyframe;
    } else {
        bool servo_exists = false;
        while (current_servo != nullptr) {
            // The given servo was already in the list, so update parameters
            if (current_servo->_servo == servo) {
                // If the given servo is already in the keyframe, update the scalar and ramp mode
                current_servo->_target_scalar = scalar;
                current_servo->_ramp_mode = ramp_mode;
                servo_exists = true;
                break;
            }
            // If this is the last servo in the list, break
            if (current_servo->_next == nullptr) {
                break;
            }
            current_servo = current_servo->_next;
        }
        if (!servo_exists) {
            current_servo->_next = new_servo_keyframe;
        } else {
            delete new_servo_keyframe; // Delete the new keyframe as it's not needed
        }
    }
}

void ServoKeyframe::start_keyframe() {
    // Iterate through the keyframe's servo keyframes and start them
    servo_node *current = _servo_head;
    while (current != nullptr) {
        // Set the ramp mode for this servo
        current->_servo->set_ramp_mode(current->_ramp_mode);
        // Set the value to ramp to for this servo
        current->_servo->set_scalar(current->_target_scalar, _duration_ms);

        current = current->_next;
    }
}

void ServoKeyframe::update_keyframe() {
    // Iterate through the keyframe's servo keyframes and update them
    servo_node *current = _servo_head;
    while (current != nullptr) {
        current->_servo->update();
        current = current->_next;
    }
}

void ServoKeyframe::set_next(ServoKeyframe *next) {
    // Set the next keyframe
    _next = next;
}

void ServoKeyframe::set_prev(ServoKeyframe *prev) {
    // Set the previous keyframe
    _prev = prev;
}

void ServoKeyframe::set_duration(unsigned long duration_ms) {
    _duration_ms = duration_ms;
}

unsigned long ServoKeyframe::get_duration() {
    return _duration_ms;
}

ServoKeyframe *ServoKeyframe::get_next() {
    return _next;
}

ServoKeyframe *ServoKeyframe::get_prev() {
    return _prev;
}

void ServoKeyframe::print_servos() const {
    Serial.println("ServoKeyframe::print_servos()");
    servo_node *current = _servo_head;
    while (current != nullptr) {
        Serial.print("Servo: ");
        Serial.println((unsigned int) current->_servo, HEX);
        current = current->_next;
    }
}