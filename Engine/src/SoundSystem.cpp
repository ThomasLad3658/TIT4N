#include "SoundSystem.hpp"

SoundSystem::SoundSystem() {
}

SoundSystem::~SoundSystem() {
    sounds.clear();
}

int SoundSystem::createSound(const std::string& filePath) {
    return createSound(filePath, SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK);
}

int SoundSystem::createSound(const std::string& filePath, SDL_AudioDeviceID dev) {
    std::unique_ptr<Sound> sound = std::make_unique<Sound>(filePath, dev);
    if (registerSound(std::move(sound))) {
        return sounds.size() - 1;
    }
    return -1;
}

void SoundSystem::update() {
    for (int i = sounds.size() - 1; i >= 0; i--) {
        sounds[i]->update();
        if (!sounds[i]->isUsable()) {
            sounds.erase(sounds.begin() + i);
        }
    }
}

bool SoundSystem::registerSound(std::unique_ptr<Sound> sound) {
    if (sound->isUsable()) {
        sounds.push_back(std::move(sound));
        return true;
    }
    return false;
}

bool SoundSystem::exists(int soundId) {
    // Fixed: was returning true when null (inverted)
    if (soundId < 0 || soundId >= (int)sounds.size()) return false;
    return sounds[soundId] != nullptr;
}

bool SoundSystem::unregisterSound(int soundId) {
    if (soundId >= 0 && soundId < (int)sounds.size()) {
        sounds[soundId] = nullptr;
        return true;
    }
    return false;
}

void SoundSystem::play(int soundId) {
    if (!exists(soundId)) return;
    return sounds[soundId]->play();
}

bool SoundSystem::isPlaying(int soundId) {
    if (!exists(soundId)) return false;
    return sounds[soundId]->isPlaying();
}

void SoundSystem::stop(int soundId) {
    if (!exists(soundId)) return;
    sounds[soundId]->stop();
}

void SoundSystem::resume(int soundId) {
    if (!exists(soundId)) return;
    sounds[soundId]->resume();
}

void SoundSystem::pause(int soundId) {
    if (!exists(soundId)) return;
    sounds[soundId]->pause();
}

void SoundSystem::loop(int soundId, bool loop) {
    if (!exists(soundId)) return;
    sounds[soundId]->loop = loop;
}