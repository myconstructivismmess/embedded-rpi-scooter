#ifndef AUDIO_H
#define AUDIO_H

#pragma once

// Standard includes
#include <string>
using std::string;

// External libraries includes
#include "SFML/Audio.hpp"
using sf::Sound;
using sf::SoundBuffer;

class Audio {
    public:
        Audio(string src) {
            buffer.loadFromFile(src);
            sound.setBuffer(buffer);
        }
        ~Audio() {
            sound.stop();
        }
        void play() {
            sound.play();       // Play queued audio
        }
        void stop() {
            sound.stop();
        }
        void setVolume(float volume) {
            sound.setVolume(volume);
        }
        void setLoop(bool loop) {
            sound.setLoop(loop);
        }
    private:
        SoundBuffer buffer;
        Sound sound;
};

#endif
