#ifndef HORNSOUNDSLOADER_H
#define HORNSOUNDSLOADER_H

#pragma once

// Standard includes
#include <string>
using std::string;

#include <filesystem>
namespace fs = std::filesystem;
using fs::path;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <stdexcept>
using std::invalid_argument;
using std::exception;

#include <vector>
using std::vector;

#include <regex>
using std::regex;

#include <exception>
using std::exception;

// External includes
#include "HornSoundPlayer.hpp"
#include "Audio.hpp"

class HornSoundsLoader {
    public:
        static void setup(string hornSoundCollectionPath) {
            _hornSoundCollectionPath = hornSoundCollectionPath;

            // Check path validity
            if (!fs::exists(hornSoundCollectionPath)) {
                throw invalid_argument("Horn sound collection path does not exist");
            } else {
                if (!fs::is_directory(hornSoundCollectionPath)) {
                    throw invalid_argument("Horn sound collection path is not a directory");
                } else {
                    cout << "HornSoundsLoader::setup: Horn sound collection path: " << hornSoundCollectionPath << endl;

                    // Load horn sounds
                    for (const auto &entry : fs::directory_iterator(_hornSoundCollectionPath)) {
                        if (!entry.is_regular_file()) {
                            continue;
                        }

                        path filePath = entry.path();
                        string fileName = filePath.filename();

                        HornSoundPreData hornSoundPreData;
                        if (!_isHornSoundFile(filePath, hornSoundPreData)) {
                            continue;
                        }

                        HornSoundPlayer::AddHornSoundData hornSoundData;
                        if (!_getHornSoundData(filePath, hornSoundPreData, hornSoundData)) {
                            continue;
                        }

                        if (HornSoundPlayer::addHornSound(hornSoundData)) {
                            cout << "HornSoundsLoader::setup: Loaded horn sound: " << fileName << endl;
                        }
                    }
                }
            }
        }
    private:
        struct HornSoundPreData {
            int pressCount;
            bool isLong;
            bool isRepeatable;
        };
        static bool _isHornSoundFile(path filePath, HornSoundPreData &hornSoundPreData) {
            // Setup sound filename regex match expressions
            const string expressionStart = "^";
            const string expressionEnd = "$";
            const string expressionFileExtension = "\\.wav";
            const string expressionNumber = "\\d+";

            const string shortHornSoundFileName = "short" + expressionNumber + expressionFileExtension;
            const string longHornSoundFileName = "long" + expressionNumber + "(_repeatable)?" + expressionFileExtension;
            const string longHornOnlyRepeatableSoundFileName = "long" + expressionNumber + "_repeatable" + expressionFileExtension;

            const regex shortHornSoundFileRegex = regex(expressionStart + shortHornSoundFileName + expressionEnd);
            const regex longHornSoundFileRegex = regex(expressionStart + longHornSoundFileName + expressionEnd);
            const regex longRepeatableHornSoundFileRegex = regex(expressionStart + longHornOnlyRepeatableSoundFileName + expressionEnd);
            const regex hornSoundFileRegex = regex(expressionStart + "(" + shortHornSoundFileName + ")|(" + longHornSoundFileName + ")" + expressionEnd);

            string fileName = filePath.filename();

            // Check if file name matches any of the sound file name patterns
            if (!regex_match(fileName, hornSoundFileRegex)) {
                return false;
            }

            // Get sound file information
            bool isLong = regex_match(fileName, longHornSoundFileRegex);
            bool isRepeatable = regex_match(fileName, longRepeatableHornSoundFileRegex);

            // Get sound file number
            size_t start = fileName.find_first_of("0123456789");
            if (start == string::npos) {
                cerr << "Something went wrong while parsing horn sound file name: " << fileName << "\tStep: Finding First Digit" << endl;
                return false;
            }

            size_t end = fileName.find_last_of("0123456789");
            if (end == string::npos) {
                cerr << "Something went wrong while parsing horn sound file name: " << fileName << "\tStep: Finding Last Digit" << endl;
                return false;
            }

            int numberIndex = -1;
            try {
                string numberString = fileName.substr(start, end - start + 1);
                numberIndex = stoi(numberString);
            } catch (const exception& e) {
                cerr << "Something went wrong while parsing horn sound file name: " << fileName << "\tStep: Parsing Number" << endl;
                return false;
            }

            if (numberIndex < 1) {
                cerr << "Horn sound file number must be greater than 0" << endl;
                return false;
            }

            hornSoundPreData.pressCount = numberIndex;
            hornSoundPreData.isLong = isLong;
            hornSoundPreData.isRepeatable = isRepeatable;

            return true;
        }
        static bool _getHornSoundData(path filePath, HornSoundPreData hornSoundPreData, HornSoundPlayer::AddHornSoundData &hornSoundData) {
            hornSoundData.pressCount = hornSoundPreData.pressCount;
            hornSoundData.isLong = hornSoundPreData.isLong;
            hornSoundData.isRepeatable = hornSoundPreData.isRepeatable;
            try {
                hornSoundData.audioPtr = new Audio(filePath);
            } catch(const exception& e) {
                delete hornSoundData.audioPtr;
                cerr << "Something went wrong while loading horn sound: " << filePath.filename() << "\tStep: Loading Sound" << endl;
                return false;
            }

            return true;
        }

        static string _hornSoundCollectionPath;
};

string HornSoundsLoader::_hornSoundCollectionPath = "";

#endif
