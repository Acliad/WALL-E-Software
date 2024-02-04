#ifndef SERVO_PLAYER_H
#define SERVO_PLAYER_H

#include "head_animation.hpp"

class ServoPlayer {
public:
    // Delete the copy constructor and assignment operator
    ServoPlayer(const ServoPlayer&) = delete;
    ServoPlayer& operator=(const ServoPlayer&) = delete;

    static ServoPlayer& getInstance();

    void play(HeadAnimation* animation);
    void stop();
    bool isPlaying();
    void update();

private:
    ServoPlayer();

    HeadAnimation* _current_animation;
};

#endif // SERVO_PLAYER_H
