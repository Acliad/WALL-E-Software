#include "motion_animations.hpp"


namespace MotionAnimations {
// Define new animations here
HeadAnimation cock_left     = HeadAnimation();
HeadAnimation cock_right    = HeadAnimation();
HeadAnimation sad           = HeadAnimation();
HeadAnimation curious_track = HeadAnimation();
HeadAnimation wiggle_eyes   = HeadAnimation();

// Create setup functions for each animation.
// NOTE: Don't forget to call these functions in setup_animations().
void setup_cock_left(ServoContext servos) {
    // cock_left cocks WALL-E's head to the left by settings the left eye to the minimum angle and the right eye to the
    // max angle
    HeadKeyframe *cock_left_keyframe_1 = new HeadKeyframe(1000);
    cock_left_keyframe_1->add_servo_scalar(servos.servo_eye_left, 1.0);
    cock_left_keyframe_1->add_servo_scalar(servos.servo_eye_right, 0.75);
    // Pause keyframe
    HeadKeyframe *cock_left_keyframe_2 = new HeadKeyframe(4000);
    // Return to neutral
    HeadKeyframe *cock_left_keyframe_3 = new HeadKeyframe(1000);
    cock_left_keyframe_3->add_servo_scalar(servos.servo_eye_left, 0.0);
    cock_left_keyframe_3->add_servo_scalar(servos.servo_eye_right, 0.0);

    cock_left.add_keyframe(cock_left_keyframe_1);
    cock_left.add_keyframe(cock_left_keyframe_2);
    cock_left.add_keyframe(cock_left_keyframe_3);
}

void setup_cock_right(ServoContext servos) {
    // cock_right cocks WALL-E's head to the left by settings the left eye to the minimum angle and the right eye to the
    // max angle
    HeadKeyframe *cock_right_keyframe_1 = new HeadKeyframe(1000);
    cock_right_keyframe_1->add_servo_scalar(servos.servo_eye_left, 1.0);
    cock_right_keyframe_1->add_servo_scalar(servos.servo_eye_right, 0.75);
    // Pause keyframe
    HeadKeyframe *cock_right_keyframe_2 = new HeadKeyframe(4000);
    // Return to neutral
    HeadKeyframe *cock_right_keyframe_3 = new HeadKeyframe(1000);
    cock_right_keyframe_3->add_servo_scalar(servos.servo_eye_left, 0.0);
    cock_right_keyframe_3->add_servo_scalar(servos.servo_eye_right, 0.0);

    cock_right.add_keyframe(cock_right_keyframe_1);
    cock_right.add_keyframe(cock_right_keyframe_2);
    cock_right.add_keyframe(cock_right_keyframe_3);
}

void setup_sad(ServoContext servos) {
    // Make WALL-E look sad by putting both eyes down, then tilting the head down
    // Eyes droop
    HeadKeyframe *sad_keyframe1 = new HeadKeyframe(2000);
    sad_keyframe1->add_servo_scalar(servos.servo_eye_left, -1.0);
    sad_keyframe1->add_servo_scalar(servos.servo_eye_right, 1.0);
    // Tilt head down
    HeadKeyframe *sad_keyframe2 = new HeadKeyframe(2000);
    sad_keyframe2->add_servo_scalar(servos.servo_neck_pitch, -0.8);
    // Pause
    HeadKeyframe *sad_keyframe3 = new HeadKeyframe(4000);
    // Reset
    HeadKeyframe *sad_keyframe4 = new HeadKeyframe(2000);
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
    HeadKeyframe *curious_track_keyframe1 = new HeadKeyframe(2000);
    // Looks down and to the left a little
    curious_track_keyframe1->add_servo_scalar(servos.servo_neck_pitch, -0.6);
    curious_track_keyframe1->add_servo_scalar(servos.servo_neck_yaw, -0.5);
    // Cock eyes
    HeadKeyframe *curious_track_keyframe2 = new HeadKeyframe(1000);
    curious_track_keyframe2->add_servo_scalar(servos.servo_eye_left, 0.7);
    curious_track_keyframe2->add_servo_scalar(servos.servo_eye_right, 0.5);
    // Track the object from left to right
    HeadKeyframe *curious_track_keyframe3 = new HeadKeyframe(6000);
    curious_track_keyframe3->add_servo_scalar(servos.servo_neck_yaw, 0.5);
    curious_track_keyframe3->add_servo_scalar(servos.servo_eye_left, 0.0);
    curious_track_keyframe3->add_servo_scalar(servos.servo_eye_right, 0.0);
    // Pause
    HeadKeyframe *curious_track_keyframe4 = new HeadKeyframe(1000);
    // Reset
    HeadKeyframe *curious_track_keyframe5 = new HeadKeyframe(1000);
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
    HeadKeyframe *wiggle_eyes_keyframe1 = new HeadKeyframe(500);
    wiggle_eyes_keyframe1->add_servo_scalar(servos.servo_eye_left, 0.5);
    wiggle_eyes_keyframe1->add_servo_scalar(servos.servo_eye_right, -0.5);
    // Wiggle other direction
    HeadKeyframe *wiggle_eyes_keyframe2 = new HeadKeyframe(500);
    wiggle_eyes_keyframe2->add_servo_scalar(servos.servo_eye_left, -0.5);
    wiggle_eyes_keyframe2->add_servo_scalar(servos.servo_eye_right, 0.5);
    // Repeat
    HeadKeyframe *wiggle_eyes_keyframe3 = new HeadKeyframe(500);
    wiggle_eyes_keyframe3->add_servo_scalar(servos.servo_eye_left, 0.5);
    wiggle_eyes_keyframe3->add_servo_scalar(servos.servo_eye_right, -0.5);
    // Wiggle other direction
    HeadKeyframe *wiggle_eyes_keyframe4 = new HeadKeyframe(500);
    wiggle_eyes_keyframe4->add_servo_scalar(servos.servo_eye_left, -0.5);
    wiggle_eyes_keyframe4->add_servo_scalar(servos.servo_eye_right, 0.5);
    // Reset
    HeadKeyframe *wiggle_eyes_keyframe5 = new HeadKeyframe(250);
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