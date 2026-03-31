#include "SoundSystem.hpp"

SoundSystem::SoundSystem() {
	SDL_AudioSpec dst;
	SDL_AudioSpec src;
	stream = SDL_CreateAudioStream(&src, &dst);
}

SoundSystem::~SoundSystem() {

}

void SoundSystem::update() {
	for (int i = 0; i < streams.size(); i++) {
		SDL_AudioStream* stream = streams[i];
		if (SDL_GetAudioStreamAvailable(stream) == 0) {
			SDL_DestroyAudioStream(stream);
			SDL_free(stream);
			streams.erase(streams.begin() + i);
		}
	}
}

int SoundSystem::playSound(const char* filePath) {
	return playSound(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, filePath);
}

int SoundSystem::playSound(SDL_AudioDeviceID device, const char* filePath) {
    SDL_AudioSpec spec;
    Uint8* buffer;
    Uint32 length;
	if (!SDL_LoadWAV(filePath, &spec, &buffer, &length)) {
		return 1;
	}

	SDL_AudioStream* stream = SDL_CreateAudioStream(&spec, &spec);

	SDL_PutAudioStreamData(stream, buffer, length);

    SDL_FlushAudioStream(stream);

	SDL_BindAudioStream(device, stream);
}