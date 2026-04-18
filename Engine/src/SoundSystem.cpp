#include "SoundSystem.hpp"

SoundSystem::SoundSystem() {
}

SoundSystem::~SoundSystem() {
	sounds.clear();
}

void SoundSystem::update() {
	for (int i = 0; i < sounds.size(); i++) {
		Sound sound = sounds[i];
		sound.update();
		if (!sound.isUsable()) sounds.erase(sounds.begin() + i);
	}
}

Sound* SoundSystem::createSound(const char* filePath) {
	return createSound(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, filePath);
}

Sound* SoundSystem::createSound(SDL_AudioDeviceID device, const char* filePath) {
    SDL_AudioSpec spec;
    Uint8* buffer;
    Uint32 length;

	if (!SDL_LoadWAV(filePath, &spec, &buffer, &length)) {
		Sound sound = Sound(SoundErrorId::LOAD_WAV);
		return &sound;
	}

	SDL_AudioStream* stream = SDL_CreateAudioStream(&spec, &spec);

	SDL_PutAudioStreamData(stream, buffer, length);

    SDL_FlushAudioStream(stream);

	Sound sound(spec, buffer, length, stream, device);
	sounds.push_back(sound);

	return &sounds[-1];
}