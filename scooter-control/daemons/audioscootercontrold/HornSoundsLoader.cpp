#include "HornSoundsLoader.hpp"

// Standard includes
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <exception>
using std::invalid_argument;
using std::exception;

#include <regex>
using std::regex_match;

// External includes
#include "HornSoundPlayer.hpp"
#include "Audio.hpp"

void HornSoundsLoader::setup(string hornSoundCollectionPath) {
    _hornSoundCollectionPath = hornSoundCollectionPath;

    // Check path validity
    if (!fs::exists(hornSoundCollectionPath)) {
        throw invalid_argument("HornSoundsLoader::setup: Horn sound collection path does not exist.");
    }

    if (!fs::is_directory(hornSoundCollectionPath)) {
        throw invalid_argument("HornSoundsLoader::setup: Horn sound collection path is not a directory.");
    }

    cout << "HornSoundsLoader::setup: Initializing horn sound collection loading with path: \"" << hornSoundCollectionPath << "\"." << endl;

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
            cout << "HornSoundsLoader::setup: Loaded horn sound: \"" << fileName << "\"." << endl;
        } else {
            cerr << "HornSoundsLoader::setup: Failed to load horn sound: \"" << fileName << "\"." << endl;
        }
    }
}

bool HornSoundsLoader::_isHornSoundFile(path filePath, HornSoundPreData &hornSoundPreData) {
    string fileName = filePath.filename();

    // Check if file name matches any of the sound file name patterns
    if (!regex_match(fileName, HORN_SOUND_FILE_REGEX)) {
        return false;
    }

    // Get sound file information
    bool isLong = regex_match(fileName, LONG_HORN_SOUND_FILE_REGEX);
    bool isRepeatable = regex_match(fileName, LONG_REPEATABLE_HORN_SOUND_FILE_REGEX);

    // Get sound file number
    size_t start = fileName.find_first_of("0123456789");
    if (start == string::npos) {
        cerr << "HornSoundsLoader::_isHornSoundFile: Something went wrong while parsing horn sound file name: " << fileName << "\tStep: \"Finding First Digit\"." << endl;
        return false;
    }

    size_t end = fileName.find_last_of("0123456789");
    if (end == string::npos) {
        cerr << "HornSoundsLoader::_isHornSoundFile: Something went wrong while parsing horn sound file name: " << fileName << "\tStep: \"Finding Last Digit\"." << endl;
        return false;
    }

    int numberIndex = -1;
    try {
        string numberString = fileName.substr(start, end - start + 1);
        numberIndex = stoi(numberString);
    } catch (const exception& e) {
        cerr << "HornSoundsLoader::_isHornSoundFile: Something went wrong while parsing horn sound file name: " << fileName << "\tStep: \"Parsing Number\"." << endl;
        return false;
    }

    if (numberIndex < 1) {
        cerr << "HornSoundsLoader::_isHornSoundFile: Something went wrong while parsing horn sound file name: " << fileName << "\tHorn sound file number must be greater than 0." << endl;
        return false;
    }

    hornSoundPreData.pressCount = numberIndex;
    hornSoundPreData.isLong = isLong;
    hornSoundPreData.isRepeatable = isRepeatable;

    return true;
}
bool HornSoundsLoader::_getHornSoundData(path filePath, HornSoundPreData hornSoundPreData, HornSoundPlayer::AddHornSoundData &hornSoundData) {
    hornSoundData.pressCount = hornSoundPreData.pressCount;
    hornSoundData.isLong = hornSoundPreData.isLong;
    hornSoundData.isRepeatable = hornSoundPreData.isRepeatable;

    try {
        hornSoundData.audioPtr = new Audio(filePath);
    } catch(const exception& e) {
        delete hornSoundData.audioPtr;
        cerr << "HornSoundsLoader::_getHornSoundData: Something went wrong while loading horn sound: " << filePath.filename() << "\tStep: \"Sound Loading\"." << endl;

        return false;
    }

    return true;
}

const string EXPRESSION_START_STR = "^";
const string EXPRESSION_END_STR = "$";
const string EXPRESSION_FILE_EXTENSION_STR = "\\.wav";
const string EXPRESSION_NUMBER_STR = "\\d+";

const string EXPRESSION_SHORT_HORN_SOUND_FILE_NAME_STR = "short" + EXPRESSION_NUMBER_STR + EXPRESSION_FILE_EXTENSION_STR;
const string EXPRESSION_LONG_HORN_SOUND_FILE_NAME_STR = "long" + EXPRESSION_NUMBER_STR + "(_repeatable)?" + EXPRESSION_FILE_EXTENSION_STR;
const string EXPRESSION_LONG_HORN_SOUND_ONLY_FILE_NAME_STR = "long" + EXPRESSION_NUMBER_STR + "_repeatable" + EXPRESSION_FILE_EXTENSION_STR;

const regex HornSoundsLoader::SHORT_HORN_SOUND_FILE_REGEX = regex(EXPRESSION_START_STR + EXPRESSION_SHORT_HORN_SOUND_FILE_NAME_STR + EXPRESSION_END_STR);
const regex HornSoundsLoader::LONG_HORN_SOUND_FILE_REGEX = regex(EXPRESSION_START_STR + EXPRESSION_LONG_HORN_SOUND_FILE_NAME_STR + EXPRESSION_END_STR);
const regex HornSoundsLoader::LONG_REPEATABLE_HORN_SOUND_FILE_REGEX = regex(EXPRESSION_START_STR + EXPRESSION_LONG_HORN_SOUND_ONLY_FILE_NAME_STR + EXPRESSION_END_STR);
const regex HornSoundsLoader::HORN_SOUND_FILE_REGEX = regex(EXPRESSION_START_STR + "(" + EXPRESSION_SHORT_HORN_SOUND_FILE_NAME_STR + ")|(" + EXPRESSION_LONG_HORN_SOUND_FILE_NAME_STR + ")" + EXPRESSION_END_STR);

string HornSoundsLoader::_hornSoundCollectionPath = "";
