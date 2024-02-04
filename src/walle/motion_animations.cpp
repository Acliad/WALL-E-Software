#include "motion_animations.hpp"


namespace MotionAnimations {
// Define new animations here
ServoAnimation cock_left     = ServoAnimation();
ServoAnimation cock_right    = ServoAnimation();
ServoAnimation sad           = ServoAnimation();
ServoAnimation curious_track = ServoAnimation();
ServoAnimation wiggle_eyes   = ServoAnimation();

// Create setup functions for each animation.
// NOTE: Don't forget to call these functions in setup_animations().
void setup_cock_left(ServoContext servos) {
    // cock_left cocks WALL-E's head to the left by settings the left eye to the minimum angle and the right eye to the
    // max angle
    ServoKeyframe *cock_left_keyframe_1 = new ServoKeyframe(1000);
    cock_left_keyframe_1->add_servo_scalar(servos.servo_eye_left, 1.0);
    cock_left_keyframe_1->add_servo_scalar(servos.servo_eye_right, 0.75);
    // Pause keyframe
    ServoKeyframe *cock_left_keyframe_2 = new ServoKeyframe(4000);
    // Return to neutral
    ServoKeyframe *cock_left_keyframe_3 = new ServoKeyframe(1000);
    cock_left_keyframe_3->add_servo_scalar(servos.servo_eye_left, 0.0);
    cock_left_keyframe_3->add_servo_scalar(servos.servo_eye_right, 0.0);

    cock_left.add_keyframe(cock_left_keyframe_1);
    cock_left.add_keyframe(cock_left_keyframe_2);
    cock_left.add_keyframe(cock_left_keyframe_3);
}

void setup_cock_right(ServoContext servos) {
    // cock_right cocks WALL-E's head to the left by settings the left eye to the minimum angle and the right eye to the
    // max angle
    ServoKeyframe *cock_right_keyframe_1 = new ServoKeyframe(1000);
    cock_right_keyframe_1->add_servo_scalar(servos.servo_eye_left, 1.0);
    cock_right_keyframe_1->add_servo_scalar(servos.servo_eye_right, 0.75);
    // Pause keyframe
    ServoKeyframe *cock_right_keyframe_2 = new ServoKeyframe(4000);
    // Return to neutral
    ServoKeyframe *cock_right_keyframe_3 = new ServoKeyframe(1000);
    cock_right_keyframe_3->add_servo_scalar(servos.servo_eye_left, 0.0);
    cock_right_keyframe_3->add_servo_scalar(servos.servo_eye_right, 0.0);

    cock_right.add_keyframe(cock_right_keyframe_1);
    cock_right.add_keyframe(cock_right_keyframe_2);
    cock_right.add_keyframe(cock_right_keyframe_3);
}

void setup_sad(ServoContext servos) {
    // Make WALL-E look sad by putting both eyes down, then tilting the head down
    // Eyes droop
    ServoKeyframe *sad_keyframe1 = new ServoKeyframe(2000);
    sad_keyframe1->add_servo_scalar(servos.servo_eye_left, -1.0);
    sad_keyframe1->add_servo_scalar(servos.servo_eye_right, 1.0);
    // Tilt head down
    ServoKeyframe *sad_keyframe2 = new ServoKeyframe(2000);
    sad_keyframe2->add_servo_scalar(servos.servo_neck_pitch, -0.8);
    // Pause
    ServoKeyframe *sad_keyframe3 = new ServoKeyframe(4000);
    // Reset
    ServoKeyframe *sad_keyframe4 = new ServoKeyframe(2000);
    sad_keyframe4->add_servo_scalar(servos.servo_eye_left, 0.0);
    sad_keyframe4->add_servo_scalar(servos.servo_eye_right, 0.0);
    sad_keyframe4->add_servo_scalar(servos.servo_neck_pitch, 0.0);

    // Add keyframes to animation
    sad.add_keyframe(sad_keyframe1);
    sad.add_keyframe(sad_keyframe2);
    sad.add_keyframe(sad_keyframe3);
    sad.add_keyframe(sad_keyframe4);
}

void setup_curious_track(ServoContext servos) {
    // Make WALL-E look like he's tracking something on the ground
    ServoKeyframe *curious_track_keyframe1 = new ServoKeyframe(2000);
    // Looks down and to the left a little
    curious_track_keyframe1->add_servo_scalar(servos.servo_neck_pitch, -0.6);
    curious_track_keyframe1->add_servo_scalar(servos.servo_neck_yaw, -0.5);
    // Cock eyes
    ServoKeyframe *curious_track_keyframe2 = new ServoKeyframe(1000);
    curious_track_keyframe2->add_servo_scalar(servos.servo_eye_left, 0.7);
    curious_track_keyframe2->add_servo_scalar(servos.servo_eye_right, 0.5);
    // Track the object from left to right
    ServoKeyframe *curious_track_keyframe3 = new ServoKeyframe(6000);
    curious_track_keyframe3->add_servo_scalar(servos.servo_neck_yaw, 0.5);
    curious_track_keyframe3->add_servo_scalar(servos.servo_eye_left, 0.0);
    curious_track_keyframe3->add_servo_scalar(servos.servo_eye_right, 0.0);
    // Pause
    ServoKeyframe *curious_track_keyframe4 = new ServoKeyframe(1000);
    // Reset
    ServoKeyframe *curious_track_keyframe5 = new ServoKeyframe(1000);
    curious_track_keyframe5->add_servo_scalar(servos.servo_neck_pitch, 0.0);
    curious_track_keyframe5->add_servo_scalar(servos.servo_neck_yaw, 0.0);
    curious_track_keyframe5->add_servo_scalar(servos.servo_eye_left, 0.0);
    curious_track_keyframe5->add_servo_scalar(servos.servo_eye_right, 0.0);

    // Add keyframes to animation
    curious_track.add_keyframe(curious_track_keyframe1);
    curious_track.add_keyframe(curious_track_keyframe2);
    curious_track.add_keyframe(curious_track_keyframe3);
    curious_track.add_keyframe(curious_track_keyframe4);
    curious_track.add_keyframe(curious_track_keyframe5);
}

void setup_wiggle_eyes(ServoContext servos) {
    // Make WALL-E wiggle his eyes in excitingment
    ServoKeyframe *wiggle_eyes_keyframe1 = new ServoKeyframe(500);
    wiggle_eyes_keyframe1->add_servo_scalar(servos.servo_eye_left, 0.5);
    wiggle_eyes_keyframe1->add_servo_scalar(servos.servo_eye_right, -0.5);
    // Wiggle other direction
    ServoKeyframe *wiggle_eyes_keyframe2 = new ServoKeyframe(500);
    wiggle_eyes_keyframe2->add_servo_scalar(servos.servo_eye_left, -0.5);
    wiggle_eyes_keyframe2->add_servo_scalar(servos.servo_eye_right, 0.5);
    // Repeat
    ServoKeyframe *wiggle_eyes_keyframe3 = new ServoKeyframe(500);
    wiggle_eyes_keyframe3->add_servo_scalar(servos.servo_eye_left, 0.5);
    wiggle_eyes_keyframe3->add_servo_scalar(servos.servo_eye_right, -0.5);
    // Wiggle other direction
    ServoKeyframe *wiggle_eyes_keyframe4 = new ServoKeyframe(500);
    wiggle_eyes_keyframe4->add_servo_scalar(servos.servo_eye_left, -0.5);
    wiggle_eyes_keyframe4->add_servo_scalar(servos.servo_eye_right, 0.5);
    // Reset
    ServoKeyframe *wiggle_eyes_keyframe5 = new ServoKeyframe(250);
    wiggle_eyes_keyframe5->add_servo_scalar(servos.servo_eye_left, 0.0);
    wiggle_eyes_keyframe5->add_servo_scalar(servos.servo_eye_right, 0.0);

    // Add keyframes to animation
    wiggle_eyes.add_keyframe(wiggle_eyes_keyframe1);
    wiggle_eyes.add_keyframe(wiggle_eyes_keyframe2);
    wiggle_eyes.add_keyframe(wiggle_eyes_keyframe3);
    wiggle_eyes.add_keyframe(wiggle_eyes_keyframe4);
    wiggle_eyes.add_keyframe(wiggle_eyes_keyframe5);
}

void setup_animations(ServoContext servos) {
    // This function should get called in the main setup() function. It calls all animation setup functions for use in
    // the main sketch.
    setup_cock_left(servos);
    setup_cock_right(servos);
    setup_sad(servos);
    setup_curious_track(servos);
    setup_wiggle_eyes(servos);
}
} // namespace MotionAnimations