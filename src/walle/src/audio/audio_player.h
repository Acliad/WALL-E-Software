#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <Arduino.h>
#include <DFMiniMp3.h>

class Mp3Notify;
// Handy typedef using serial and our notify class
typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3;

/**
 * @brief The Mp3Notify class provides static methods for handling MP3 player events.
 */
class Mp3Notify {
  public:
    /**
     * @brief Prints the source and action of the MP3 player event.
     *
     * @param source The play source(s) of the MP3 player event.
     * @param action The action performed by the MP3 player event.
     */
    static void PrintlnSourceAction(DfMp3_PlaySources source, const char *action);
    /**
     * @brief Handles the error event of the MP3 player.
     *
     * @param mp3 The MP3 player instance.
     * @param errorCode The error code indicating the type of error.
     */
    static void OnError([[maybe_unused]] DfMp3 &mp3, uint16_t errorCode);
    /**
     * @brief Handles the play finished event of the MP3 player.
     *
     * @param mp3 The MP3 player instance.
     * @param source The play source(s) of the MP3 player event.
     * @param track The track number that finished playing.
     */
    static void OnPlayFinished([[maybe_unused]] DfMp3 &mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track);
    /**
     * @brief Handles the play source online event of the MP3 player.
     *
     * @param mp3 The MP3 player instance.
     * @param source The play source(s) of the MP3 player event.
     */
    static void OnPlaySourceOnline([[maybe_unused]] DfMp3 &mp3, DfMp3_PlaySources source);
    /**
     * @brief Handles the play source inserted event of the MP3 player.
     *
     * @param mp3 The MP3 player instance.
     * @param source The play source(s) of the MP3 player event.
     */
    static void OnPlaySourceInserted([[maybe_unused]] DfMp3 &mp3, DfMp3_PlaySources source);
    /**
     * @brief Handles the play source removed event of the MP3 player.
     *
     * @param mp3 The MP3 player instance.
     * @param source The play source(s) of the MP3 player event.
     */
    static void OnPlaySourceRemoved([[maybe_unused]] DfMp3 &mp3, DfMp3_PlaySources source);
};

#endif // AUDIO_PLAYER_H
