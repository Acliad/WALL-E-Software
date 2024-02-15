#include "animate_servo.hpp"

ServoAnimation::ServoAnimation()
    : _head(nullptr), _current_keyframe(nullptr), _frame_start_time_ms(0), _playing(false),
      _keyframe_has_started(false) {
}

ServoAnimation::~ServoAnimation() {
    // Iterate through the keyframes and delete them
    ServoKeyframe *current = _head;
    while (current != nullptr) {
        ServoKeyframe *next = current->get_next();
        delete current;
        current = next;
    }
}

void ServoAnimation::add_keyframe(ServoKeyframe *keyframe) {
    // Find the first keyframe relative to the given keyframe
    ServoKeyframe *given_head = keyframe;
    while (given_head->get_prev() != nullptr) {
        given_head = given_head->get_prev();
    }
    // Find the end of the linked list and add the new keyframe
    ServoKeyframe *current = _head;
    if (current == nullptr) {
        _head = given_head;
    } else {
        while (current->get_next() != nullptr) {
            current = current->get_next();
        }
        current->set_next(given_head);
        given_head->set_prev(current);
    }
}

void ServoAnimation::play() {
    // Start the animation
    _playing = true;
    _current_keyframe = _head;
    _keyframe_has_started = false;
    _frame_start_time_ms = millis();
}

void ServoAnimation::stop() {
    // Stop the animation
    _playing = false;
    _current_keyframe = _head;
}

void ServoAnimation::update() {
    // Check if the animation is running, if not, stop
    if (!_playing) {
        return;
    }

    // Check if we reached the end of the animation, if so, stop
    if (_current_keyframe == nullptr) {
        stop();
        return;
    }

    // Check if the current keyframe has started, if not, start it
    if (!_keyframe_has_started) {
        // Start the keyframe
        _keyframe_has_started = true;

        // Set the start time
        _frame_start_time_ms = millis();

        // Setup all the servos in this keyframe
        _current_keyframe->start_keyframe();
    } // Check if we reached the end of the current keyframe, if so, move to the next keyframe
    else if (millis() - _frame_start_time_ms > _current_keyframe->get_duration()) {
        // Do one last update to make sure we got the tail end of the ramp
        _current_keyframe->update_keyframe();
        _current_keyframe = _current_keyframe->get_next();
        _keyframe_has_started = false;
        return;
    }

    // Otherwise we're in the middle of a keyframe, update it
    _current_keyframe->update_keyframe();
}

bool ServoAnimation::isPlaying() {
    return _playing;
}

void ServoAnimation::printDebugInfo() {
    Serial.println("--------- ServoAnimation ---------");
    Serial.print("Playing: ");
    Serial.println(_playing);
    Serial.print("Head Keyframe: ");
    Serial.println((unsigned int) _head, HEX);
    Serial.print("Current Keyframe: ");
    Serial.println((unsigned int) _current_keyframe, HEX);
    if (_head != nullptr) {
        Serial.print("Head Keyframe Duration: ");
        Serial.println(_head->get_duration());
        if (_head->get_next() != nullptr) {
            Serial.print("Next Keyframe Duration: ");
            Serial.println(_head->get_next()->get_duration());
        }
    }
}

bool ServoAnimation::save(fs::FS &filesystem, const char *filename) {
    File animation_file = filesystem.open(filename, FILE_WRITE);
    if (!animation_file) {
        Serial.println("Failed to open file for writing");
        return false;
    }

    // Loop through the keyframes and write each one to the file
    ServoKeyframe *current = _head;
    while (current != nullptr) {
        bool success = true;
        success &= 0 < animation_file.println("start keyframe");
        success &= 0 < animation_file.println(current->serialize().c_str());
        success &= 0 < animation_file.println("end keyframe");
        if (!success) {
            Serial.println("Write failed");
            animation_file.close();
            return false;
        }
        current = current->get_next();
    }

    animation_file.close();
    return true;
}

std::unique_ptr<ServoAnimation> ServoAnimation::load(fs::FS &filesystem, const char* filename, ServoContext& servo_context) {
    File animation_file = filesystem.open(filename, FILE_READ);
    if (!animation_file) {
        Serial.println("Failed to open file for reading");
        return nullptr;
    }

    // Read the keyframes from the file and create a new animation from the data
    std::unique_ptr<ServoAnimation> animation(new ServoAnimation());
    String                          keyframe_str;
    while (animation_file.available()) {
        String line = animation_file.readStringUntil('\n');
        // DEBUG >>>>>>>>>>>>>>>>>>>
        Serial.println(line);
        // DEBUG <<<<<<<<<<<<<<<<<<<<
        if (line.indexOf("start keyframe") > -1) {
            keyframe_str = "";
        } else if (line.indexOf("end keyframe") > -1) {
            ServoKeyframe *keyframe = ServoKeyframe::deserialize(keyframe_str.c_str(), servo_context);
            if (keyframe != nullptr) {
                animation->add_keyframe(keyframe);
            }
        } else {
            keyframe_str += line;
        }
    }

    animation_file.close();
    return animation;
}