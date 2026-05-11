#include "SoundSystem.hpp"

SoundSystem::SoundSystem() {
}

SoundSystem::~SoundSystem() {
    sounds.clear();
}

int SoundSystem::createSound(const std::string& filePath) {
    return createSoundWithDev(filePath, SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK);
}

int SoundSystem::createSoundWithDev(const std::string& filePath, SDL_AudioDeviceID dev) {
    auto sound = std::make_unique<Sound>(filePath, dev);
    if (!sound->isUsable()) return -1;

    // Reuse a null slot if available
    for (int i = 0; i < (int)sounds.size(); i++) {
        if (!sounds[i]) {
            sounds[i] = std::move(sound);
            return i;
        }
    }

    sounds.push_back(std::move(sound));
    return (int)sounds.size() - 1;
}

void SoundSystem::update() {
    for (auto& sound : sounds) {
        if (sound) sound->update();
    }
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
    sounds[soundId]->play();
}

void SoundSystem::playFrom(int soundId, int startMs) {
    if (!exists(soundId)) return;
    sounds[soundId]->play(startMs);
}

void SoundSystem::playFromTo(int soundId, int startMs, int endMs) {
    if (!exists(soundId)) return;
    sounds[soundId]->play(startMs, endMs);
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

Uint64 SoundSystem::getStartTick(int soundId) {
    if (!exists(soundId)) return 0;
    return sounds[soundId]->getStartTick();
}

Uint64 SoundSystem::getEndTick(int soundId) {
    if (!exists(soundId)) return 0;
    return sounds[soundId]->getEndTick();
}