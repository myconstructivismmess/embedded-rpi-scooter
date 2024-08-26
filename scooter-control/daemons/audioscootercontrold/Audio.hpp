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
        Audio(string src);
        ~Audio();

        void play();
        void stop();

        void setVolume(float volume);
        
        void setLoop(bool loop);
    private:
        SoundBuffer buffer;
        Sound sound;
};

#endif
