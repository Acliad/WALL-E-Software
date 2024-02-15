#include "servo_keyframe.hpp"

ServoKeyframe::ServoKeyframe(unsigned long duration_ms)
    : _next(nullptr), _prev(nullptr), _servo_head(nullptr), _duration_ms(duration_ms), _function(nullptr),
      _function_has_fired(false) {
}

ServoKeyframe::ServoKeyframe(const ServoKeyframe &keyframe) {
    _next = nullptr;
    _prev = nullptr;
    _servo_head = nullptr;
    _function = nullptr;
    _function_has_fired = false;

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

void ServoKeyframe::add_function(std::function<void()> function) {
    _function = function;
}

void ServoKeyframe::start_keyframe() {
    _function_has_fired = false;
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
    // Check if the function has fired, if not, fire it
    if (_function != nullptr && !_function_has_fired) {
        _function();
        _function_has_fired = true;
    }
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

std::string ServoKeyframe::serialize() const {
    // NOTE: This could be memory inefficent because it's creating a potentially large string in memory instead of 
    // writing directly to SPIFFS. If this becomes a problem, we can change this to write directly to SPIFFS.
    std::string output_str;
    output_str += "duration_ms: " + std::to_string(_duration_ms) + "\n";
    // TODO: Serialize the function
    // Iterate through the keyframe's servo keyframes and serialize them
    servo_node *current = _servo_head;
    while (current != nullptr) {
        output_str += "servo: " + current->_servo->get_name() + "\n";
        output_str += "target_scalar: " + std::to_string(current->_target_scalar) + "\n";
        output_str += "ramp_mode: " + std::to_string(current->_ramp_mode) + "\n";
        current = current->_next;
    }

    return output_str;
}

ServoKeyframe *ServoKeyframe::deserialize(std::string keyframe_string, ServoContext &servo_context) {
    // Creates a keyframe from the given string. Keyframes are stored as text data with the format:
    // duration_ms: <duration_ms>
    //  servo: <servo_name0>
    //  target_scalar: <target_scalar0>
    //  ramp_mode: <ramp_mode0>
    //  servo: <servo_name1>
    //  target_scalar: <target_scalar1>
    //  ramp_mode: <ramp_mode1>
    //  ...

    ServoKeyframe *keyframe = new ServoKeyframe(0);
    // Split the string into lines
    std::istringstream keyframe_stream(keyframe_string);

    unsigned long duration_ms;
    struct ServoData {
        std::string servo_name;
        float       target_scalar;
        ramp_mode   ramp_mode_;
    };
    std::vector<ServoData> servo_data;
    for (std::string line; std::getline(keyframe_stream, line);) {
        // Split the line into the key and value
        std::istringstream line_stream(line);
        std::string        key;
        std::string        value;
        std::getline(line_stream, key, ':');
        std::getline(line_stream, value);

        // Remove any leading or trailing whitespace
        key.erase(0, key.find_first_not_of(" \t")); // Erase white space at the beginning
        key.erase(key.find_last_not_of(" \t") + 1); // Erase white space at the end
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        // Check the key and set the value
        if (key == "duration_ms") {
            // Get the duration of the new keyframe
            duration_ms = std::stoul(value);
            keyframe->set_duration(duration_ms);
        } else if (key == "servo") {
            // New servo
            servo_data.push_back(ServoData());
            servo_data.back().servo_name = value;
        } else if (key == "target_scalar") {
            servo_data.back().target_scalar = std::stof(value);
        } else if (key == "ramp_mode") {
            servo_data.back().ramp_mode_ = static_cast<ramp_mode>(std::stoi(value));
        }
    }

    // Add the servos to the keyframe
    for (auto &data : servo_data) {
        ServoMotor *servo = servo_context.getByName(data.servo_name);
        if (servo == nullptr) {
            Serial.println("Servo not found");
            continue;
        }
        keyframe->add_servo_scalar(servo, data.target_scalar, data.ramp_mode_);
    }

    return keyframe;
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