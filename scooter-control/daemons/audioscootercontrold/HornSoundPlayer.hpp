#ifndef HORNSOUNDPLAYER_H
#define HORNSOUNDPLAYER_H

#pragma once

// Standard includes
#include <map>
using std::map;

#include <string>
using std::string;

// External includes
#include "Audio.hpp"

class HornSoundPlayer {
    public:
        struct AddHornSoundData {
            unsigned int pressCount;
            bool isLong;
            bool isRepeatable;
            Audio* audioPtr;
        };
        static bool addHornSound(AddHornSoundData addHornSoundData);
        static void removeHornSound(unsigned int pressCount, bool isLong);

        static void playHornSound(unsigned int pressCount, bool isLong);
        static void stopHornSound(bool log = true, string functionName = "stopHornSound");

        static unsigned int getLastHornPressCount();
        static bool isLastHornSoundLong();
        static bool isLongHornSoundRepeatable(unsigned int pressCount);
    private:
        static void _removeSoundBySoundIndex(unsigned int soundIndex);

        static unsigned int _pressCountToSoundIndex(unsigned int pressCount, bool isLong);

        struct SoundData {
            unsigned int pressCount;
            bool isLong;
            bool isRepeatable;
            Audio* audioPtr;
        };

        static map<unsigned int, SoundData> _soundsData;
        static int _playingSoundIndex;
};

#endif
