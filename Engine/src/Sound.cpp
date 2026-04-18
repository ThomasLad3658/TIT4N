#include "Sound.hpp"

Sound::Sound(SoundErrorId errId) :
	audioSpec(), buffer(nullptr), length(0), stream(nullptr), device(), startTick(-1), playing(false), usable(false), errorId(errId)
{
}

Sound::Sound(SDL_AudioSpec spec, Uint8* buf, Uint32 len, SDL_AudioStream* s, SDL_AudioDeviceID dev)
	: audioSpec(spec), buffer(buf), length(len), stream(s), device(dev), startTick(-1), playing(false), usable(true), errorId(0)
{}

Sound::~Sound() {
	if (usable) {
		SDL_ClearAudioStream(stream);
		SDL_DestroyAudioStream(stream);
		SDL_free(stream);
		usable = false;
	}
}

int Sound::getErrorId() const {
	return errorId;
}

int Sound::getStartTick() const {
	return startTick;
}

bool Sound::isUsable() const {
	return usable;
}

bool Sound::isPlaying() const {
	return playing;
}

void Sound::play() {
	if (usable && !playing) {
		startTick = SDL_GetTicks();
		SDL_BindAudioStream(device, stream);
		playing = true;
	}
}

void Sound::update() {
	if (playing && usable && SDL_GetAudioStreamAvailable(stream) == 0) {
		SDL_ClearAudioStream(stream);
		SDL_DestroyAudioStream(stream);
		SDL_free(stream);
		usable = false;
		playing = false;
	}
}

void Sound::stop() {
	SDL_ClearAudioStream(stream);
	SDL_DestroyAudioStream(stream);
	SDL_free(stream);
	playing = false;
	usable = false;
}