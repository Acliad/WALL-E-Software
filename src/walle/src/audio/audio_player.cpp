/**
 * @file audio_player.cpp
 * @author Isaac Rex (@Acliad)
 * @brief This file contains the implementation for the Mp3Notify class. This class is used to handle notification
 * events from the DfMp3 library like a file finished playing.
 * @version 0.1
 * @date 2024-02-19
 *
 *
 */
#include "audio_player.hpp"

void Mp3Notify::PrintlnSourceAction(DfMp3_PlaySources source, const char *action) {
    if (source & DfMp3_PlaySources_Sd) {
        Serial.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb) {
        Serial.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash) {
        Serial.print("Flash, ");
    }
    Serial.println(action);
}

void Mp3Notify::OnError(DfMp3 &mp3, uint16_t errorCode) {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
}

void Mp3Notify::OnPlayFinished(DfMp3 &mp3, DfMp3_PlaySources source, uint16_t track) {
    Serial.print("Play finished for #");
    Serial.println(track);
}

void Mp3Notify::OnPlaySourceOnline(DfMp3 &mp3, DfMp3_PlaySources source) {
    PrintlnSourceAction(source, "online");
}

void Mp3Notify::OnPlaySourceInserted(DfMp3 &mp3, DfMp3_PlaySources source) {
    PrintlnSourceAction(source, "inserted");
}

void Mp3Notify::OnPlaySourceRemoved(DfMp3 &mp3, DfMp3_PlaySources source) {
    PrintlnSourceAction(source, "removed");
}