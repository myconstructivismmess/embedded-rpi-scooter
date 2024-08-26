#include "Audio.hpp"

Audio::Audio(string src) {
    buffer.loadFromFile(src);
    sound.setBuffer(buffer);
}
Audio::~Audio() {
    sound.stop();
}

void Audio::play() {
    sound.play();
}
void Audio::stop() {
    sound.stop();
}

void Audio::setVolume(float volume) {
    sound.setVolume(volume);
}

void Audio::setLoop(bool loop) {
    sound.setLoop(loop);
}
