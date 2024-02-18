#ifndef SERVO_PLAYER_H
#define SERVO_PLAYER_H

#include "animate_servo.hpp"

class ServoPlayer {
public:
    // Delete the copy constructor and assignment operator
    ServoPlayer(const ServoPlayer&) = delete;
    ServoPlayer& operator=(const ServoPlayer&) = delete;

    static ServoPlayer& getInstance();

    void play(ServoAnimation* animation);
    void stop();
    bool isPlaying();
    void update();
    ServoAnimation* getCurrentAnimation();

private:
    ServoPlayer();

    ServoAnimation* _current_animation;
    bool _is_playing;
};

#endif // SERVO_PLAYER_H
