#ifndef HORNSOUNDSLOADER_H
#define HORNSOUNDSLOADER_H

#pragma once

// Standard includes
#include <string>
using std::string;

#include <regex>
using std::regex;

#include <filesystem>
namespace fs = std::filesystem;
using fs::path;

class HornSoundsLoader {
    public:
        static void setup(string hornSoundCollectionPath);
    private:
        struct HornSoundPreData {
            int pressCount;
            bool isLong;
            bool isRepeatable;
        };
        static bool _isHornSoundFile(path filePath, HornSoundPreData &hornSoundPreData);
        static bool _getHornSoundData(path filePath, HornSoundPreData hornSoundPreData, HornSoundPlayer::AddHornSoundData &hornSoundData);

        static const regex SHORT_HORN_SOUND_FILE_REGEX;
        static const regex LONG_HORN_SOUND_FILE_REGEX;
        static const regex LONG_REPEATABLE_HORN_SOUND_FILE_REGEX;
        static const regex HORN_SOUND_FILE_REGEX;

        static string _hornSoundCollectionPath;
};

#endif
