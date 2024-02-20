/**
 * @file servo_keyframe.hpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the declaration of the ServoKeyframe class, which is used to create animations by chaining
 * together keyframes. This class is intended to be used with the ServoAnimation class. Provides an interface for
 * setting keyframes as servo positions, ramp curve, and duration of the keyframe.
 * @version 0.1
 * @date 2023-10-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SERVO_KEYFRAME_HPP
#define SERVO_KEYFRAME_HPP

#include <sstream>
#include <string>
#include <iostream>
#include <Arduino.h>
#include <Ramp.h>
#include <vector>
#include "servo_motor.hpp"
#include "servo_context.hpp"
#include "../audio/audio_player.hpp"

/**
 * @brief Represents a keyframe for controlling servo motors.
 * 
 * This class provides methods for adding servo angles and scalars, setting up ramps, and managing keyframe duration.
 * It also supports adding tracks to play at the start of the keyframe.
 * The keyframe can be serialized and deserialized for storage or transmission.
 * 
 */
class ServoKeyframe {
  public:
    /**
     * @brief Constructs a ServoKeyframe object with the specified duration.
     * 
     * @param duration_ms The duration of the keyframe in milliseconds.
     */
    ServoKeyframe(unsigned long duration_ms);

    /**
     * @brief Copy constructor for ServoKeyframe objects. The copy constructor will not copy the next and previous
     * pointers of the keyframe, as it is intended to be used for creating a new keyframe from an existing one. It will
     * copy the DfMp3 pointer, track index, and pointers to the servos.
     *
     * @param keyframe The ServoKeyframe object to be copied.
     */
    ServoKeyframe(const ServoKeyframe &keyframe);

    /**
     * @brief Destructor for ServoKeyframe objects. Deletes this keyframe and stiches the linked list back together, if
     * necessary.
     */
    ~ServoKeyframe();

    /**
     * @brief Adds a servo the keyframe, final position defined by angle.
     * 
     * @param servo The ServoMotor object to control.
     * @param angle The target angle for the servo.
     * @param ramp_mode The ramp mode for the servo motion (default: QUADRATIC_INOUT).
     */
    void add_servo_angle(ServoMotor *servo, float angle, ramp_mode ramp_mode = QUADRATIC_INOUT);

    /**
     * @brief Adds a servo to the keyframe, final position defined by scalar.
     * 
     * @param servo The ServoMotor object to control.
     * @param scalar The target scalar for the servo.
     * @param ramp_mode The ramp mode for the servo motion (default: QUADRATIC_INOUT).
     */
    void add_servo_scalar(ServoMotor *servo, float scalar, ramp_mode ramp_mode = QUADRATIC_INOUT);

    /**
     * @brief Adds a track to play at the start of the keyframe.
     * 
     * @param track_index The index of the track to play.
     * @param dfmp3 The DfMp3 object for playing the track.
     */
    void add_track(int track_index, DfMp3 *dfmp3);

    /**
     * @brief Sets up the ramp for all the servos in this keyframe and starts them.
     */
    void start_keyframe();

    /**
     * @brief Updates the internal ramp state for all the servos in this keyframe.
     */
    void update();
    
    /**
     * @brief Sets the duration of the keyframe.
     * 
     * @param duration_ms The duration of the keyframe in milliseconds.
     */
    void set_duration(unsigned long duration_ms);

    /**
     * @brief Gets the duration of the keyframe.
     * 
     * @return The duration of the keyframe in milliseconds.
     */
    unsigned long get_duration();

    /**
     * @brief Sets the next keyframe in the linked list.
     * 
     * @param next The next ServoKeyframe object.
     */
    void set_next(ServoKeyframe *next);

    /**
     * @brief Gets the next keyframe in the linked list.
     * 
     * @return The next ServoKeyframe object.
     */
    ServoKeyframe *get_next();

    /**
     * @brief Sets the previous keyframe in the linked list.
     * 
     * @param prev The previous ServoKeyframe object.
     */
    void set_prev(ServoKeyframe *prev);

    /**
     * @brief Gets the previous keyframe in the linked list.
     * 
     * @return The previous ServoKeyframe object.
     */
    ServoKeyframe *get_prev();

    /**
     * @brief Serializes the keyframe into a string representation.
     * 
     * @return The serialized keyframe string.
     */
    std::string serialize() const;

    /**
     * @brief Deserializes a keyframe string and creates a ServoKeyframe object.
     * 
     * @param keyframe_string The serialized keyframe string.
     * @param servo_context The ServoContext object for servo management.
     * @param dfmp3 The DfMp3 object for playing tracks.
     * @return The deserialized ServoKeyframe object.
     */
    static ServoKeyframe *deserialize(std::string keyframe_string, ServoContext& servo_context, DfMp3 *dfmp3);

    /**
     * @brief Prints the servos in the keyframe for debugging purposes.
     */
    void print_servos() const;

  private:
    /**
     * @brief Represents a node in the linked list of servos.
     */
    struct servo_node {
        ServoMotor *_servo;
        float       _target_scalar;
        ramp_mode   _ramp_mode;
        servo_node *_next;
    };

    servo_node *_servo_head; /**< The head of the linked list of servos. */
    unsigned long _duration_ms; /**< The duration of the keyframe in milliseconds. */
    ServoKeyframe *_next; /**< The next keyframe in the linked list. */
    ServoKeyframe *_prev; /**< The previous keyframe in the linked list. */
    int _track_index; /**< The index of the track to play at the start of the keyframe. */
    bool _track_has_played; /**< Flag indicating whether the track has played. */
    DfMp3 *_dfmp3; /**< The DfMp3 object for playing tracks. */
};

#endif // SERVO_KEYFRAME_HPP