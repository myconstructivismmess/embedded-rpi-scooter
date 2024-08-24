#ifndef HORNSOUNDPLAYER_H
#define HORNSOUNDPLAYER_H

#pragma once

// Standard includes
#include <map>
using std::map;

#include <stdexcept>
using std::invalid_argument;

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

// External includes
#include "Audio.hpp"

class HornSoundPlayer {
    public:
        ~HornSoundPlayer() {
            for (auto const& [soundIndex, hornSoundData] : _hornSoundsData) {
                _removeHornSoundBySoundIndex(soundIndex);
            }
        }

        struct AddHornSoundData {
            unsigned int pressCount;
            bool isLong;
            bool isRepeatable;
            Audio* audioPtr;
        };
        static bool addHornSound(AddHornSoundData addHornSoundData) {
            unsigned int soundIndex = _pressCountToSoundIndex(
                addHornSoundData.pressCount,
                addHornSoundData.isLong
            );

            if (_hornSoundsData.contains(soundIndex)) {
                cerr << "HornSoundPlayer::addHornSound: Horn sound index " << soundIndex << " already exists" << endl;
                delete addHornSoundData.audioPtr;
                return false;
            }

            addHornSoundData.audioPtr->setLoop(addHornSoundData.isRepeatable);
            addHornSoundData.audioPtr->setVolume(100);

            _hornSoundsData.insert({
                soundIndex,
                HornSoundData {
                    pressCount: addHornSoundData.pressCount,
                    isLong: addHornSoundData.isLong,
                    isRepeatable: addHornSoundData.isRepeatable,
                    audioPtr: addHornSoundData.audioPtr
                }
            });

            return true;
        }
        static void removeHornSound(unsigned int pressCount, bool isLong) {
            unsigned int soundIndex = _pressCountToSoundIndex(pressCount, isLong);

            _removeHornSoundBySoundIndex(soundIndex);
        }

        static void playHornSound(unsigned int pressCount, bool isLong) {
            unsigned int soundIndex = _pressCountToSoundIndex(
                pressCount,
                isLong
            );
            
            stopHornSound(false);

            if (!_hornSoundsData.contains(soundIndex)) {
                cerr << "HornSoundPlayer::playHornSound: Horn sound index " << soundIndex << " not found" << endl;
                return;
            }

            cout << "HornSoundPlayer::playHornSound: Playing horn sound " << soundIndex << " isLong: " << isLong << endl;

            _hornSoundsData[soundIndex].audioPtr->play();
            _playingSoundIndex = soundIndex;
        }
        static void stopHornSound(bool externalCall = true) {
            if (!_hornSoundsData.contains(_playingSoundIndex)) {
                return;
            }

            _hornSoundsData[_playingSoundIndex].audioPtr->stop();
            
            _playingSoundIndex = -1;

            if (externalCall) {
                cout << "HornSoundPlayer::playHornSound: Stopping horn sound" << endl;
            }
        }

        static unsigned int getLastHornPressCount() {
            unsigned int maxPressCount = 0;

            for (auto const& [soundIndex, hornSoundData] : _hornSoundsData) {
                if (hornSoundData.pressCount > maxPressCount) {
                    maxPressCount = hornSoundData.pressCount;
                }
            }

            return maxPressCount;
        }
        static bool isLastHornSoundLong() {
            unsigned int lastHornPressCount = getLastHornPressCount();

            bool isLastSoundLong = false;
            for (auto const& [soundIndex, hornSoundData] : _hornSoundsData) {
                if (hornSoundData.pressCount == lastHornPressCount && hornSoundData.isLong) {
                    isLastSoundLong = true;
                    break;
                }
            }

            return isLastSoundLong;
        }
        static bool isLongHornSoundRepeatable(unsigned int pressCount) {
            unsigned int soundIndex = _pressCountToSoundIndex(pressCount, true);

            if (!_hornSoundsData.contains(soundIndex)) {
                return false;
            }

            return _hornSoundsData[soundIndex].isRepeatable;
        }
    private:
        static void _removeHornSoundBySoundIndex(unsigned int soundIndex) {
            if (!_hornSoundsData.contains(soundIndex)) {
                cerr << "HornSoundPlayer::removeHornSound: Horn sound index " << soundIndex << " not found" << endl;
                return;
            }

            if (_playingSoundIndex == soundIndex) {
                stopHornSound();
            }

            // Remove audio from the heap
            delete _hornSoundsData[soundIndex].audioPtr;
            
            _hornSoundsData.erase(soundIndex);
        }

        static unsigned int _pressCountToSoundIndex(unsigned int pressCount, bool isLong) {
            return isLong
                ? (pressCount - 1) * 2 + 1
                : (pressCount - 1) * 2;
        }

        struct HornSoundData {
            unsigned int pressCount;
            bool isLong;
            bool isRepeatable;
            Audio* audioPtr;
        };
        static map<unsigned int, HornSoundData> _hornSoundsData;
        static int _playingSoundIndex;
};

map<unsigned int, HornSoundPlayer::HornSoundData> HornSoundPlayer::_hornSoundsData = map<unsigned int, HornSoundPlayer::HornSoundData>();
int HornSoundPlayer::_playingSoundIndex = 0;

#endif
