#include "HornSoundPlayer.hpp"

// Standard includes
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

bool HornSoundPlayer::addHornSound(AddHornSoundData addHornSoundData) {
    unsigned int soundIndex = _pressCountToSoundIndex(
        addHornSoundData.pressCount,
        addHornSoundData.isLong
    );

    if (_soundsData.contains(soundIndex)) {
        cerr << "HornSoundPlayer::addHornSound: Horn sound with index " << soundIndex << " already exists." << endl;
        delete addHornSoundData.audioPtr;
        return false;
    }

    addHornSoundData.audioPtr->setLoop(addHornSoundData.isRepeatable);
    addHornSoundData.audioPtr->setVolume(100);

    _soundsData.insert({
        soundIndex,
        SoundData {
            pressCount: addHornSoundData.pressCount,
            isLong: addHornSoundData.isLong,
            isRepeatable: addHornSoundData.isRepeatable,
            audioPtr: addHornSoundData.audioPtr
        }
    });

    return true;
}
void HornSoundPlayer::removeHornSound(unsigned int pressCount, bool isLong) {
    unsigned int soundIndex = _pressCountToSoundIndex(pressCount, isLong);
    _removeSoundBySoundIndex(soundIndex);
}

void HornSoundPlayer::playHornSound(unsigned int pressCount, bool isLong) {
    unsigned int soundIndex = _pressCountToSoundIndex(
        pressCount,
        isLong
    );

    stopHornSound(false);

    if (!_soundsData.contains(soundIndex)) {
        cerr << "HornSoundPlayer::playHornSound: Horn sound with index " << soundIndex << " not found." << endl;
        return;
    }

    cout << "HornSoundPlayer::playHornSound: Playing horn sound with index " << soundIndex << "." << endl;

    _soundsData[soundIndex].audioPtr->play();
    _playingSoundIndex = soundIndex;
}
void HornSoundPlayer::stopHornSound(bool log, string functionName) {
    if (!_soundsData.contains(_playingSoundIndex)) {
        return;
    }

    if (log) {
        cout << "HornSoundPlayer::" << functionName << ": Stopping horn sound." << endl;
    }

    _soundsData[_playingSoundIndex].audioPtr->stop();
    _playingSoundIndex = -1;
}

unsigned int HornSoundPlayer::getLastHornPressCount() {
    unsigned int maxPressCount = 0;
    for (auto const& [soundIndex, soundData] : _soundsData) {
        if (soundData.pressCount > maxPressCount) {
            maxPressCount = soundData.pressCount;
        }
    }

    return maxPressCount;
}
bool HornSoundPlayer::isLastHornSoundLong() {
    unsigned int lastPressCount = getLastHornPressCount();
    bool isLastSoundLong = false;
    for (auto const& [soundIndex, soundData] : _soundsData) {
        if (soundData.pressCount == lastPressCount && soundData.isLong) {
            isLastSoundLong = true;
            break;
        }
    }

    return isLastSoundLong;
}
bool HornSoundPlayer::isLongHornSoundRepeatable(unsigned int pressCount) {
    unsigned int soundIndex = _pressCountToSoundIndex(pressCount, true);

    if (!_soundsData.contains(soundIndex)) {
        cerr << "HornSoundPlayer::isLongHornSoundRepeatable: Horn sound with index " << soundIndex << " not found." << endl;
        return false;
    }

    return _soundsData[soundIndex].isRepeatable;
}

void HornSoundPlayer::_removeSoundBySoundIndex(unsigned int soundIndex) {
    if (!_soundsData.contains(soundIndex)) {
        cerr << "HornSoundPlayer::_removeHornSoundBySoundIndex: Horn sound with index " << soundIndex << " not found." << endl;
        return;
    }

    if (_playingSoundIndex == soundIndex) {
        stopHornSound(true, "_removeSoundBySoundIndex");
    }

    delete _soundsData[soundIndex].audioPtr;
    _soundsData.erase(soundIndex);
}

unsigned int HornSoundPlayer::_pressCountToSoundIndex(unsigned int pressCount, bool isLong) {
    return isLong
        ? (pressCount - 1) * 2 + 1
        : (pressCount - 1) * 2;
}

map<unsigned int, HornSoundPlayer::SoundData> HornSoundPlayer::_soundsData = map<unsigned int, HornSoundPlayer::SoundData>();
int HornSoundPlayer::_playingSoundIndex = 0;
