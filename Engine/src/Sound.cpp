#include "Sound.hpp"

void Sound::cleanUp() {
	usable = false;
	if (stream) {
		SDL_DestroyAudioStream(stream);
		stream = nullptr;
	}
	if (buffer) {
		SDL_free(buffer);
		buffer = nullptr;
	}
}

void Sound::init(std::string filePath) {
	if (!SDL_LoadWAV(filePath.c_str(), &spec, &buffer, &length)) {
		cleanUp();
		errorId = SoundErrorId::LOAD_WAV;
		return;
	}
	stream = SDL_OpenAudioDeviceStream(device, &spec, nullptr, nullptr);
	if (!stream) {
		cleanUp();
		errorId = SoundErrorId::LOAD_DEVICE;
		return;
	}
}

Sound::Sound(std::string filePath, SDL_AudioDeviceID dev)
	: device(dev), buffer(nullptr), stream(nullptr), startTick(-1), playing(false), paused(false), usable(true), errorId(0), duration(0.0f)
{
	init(filePath);
}

Sound::~Sound() {
	cleanUp();
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
	if (!usable || !buffer) return;
	SDL_PutAudioStreamData(stream, buffer, (int)length);
	SDL_ResumeAudioStreamDevice(stream);
	playing = true;
	paused = false;
	startTick = SDL_GetTicks();
}


void Sound::update() {
	if (playing) {
		// Check if stream is done
		if (SDL_GetAudioStreamAvailable(stream) == 0) {
			if (loop == true) play();
			else stop();
		}
	}
}

void Sound::stop() {
	if (playing) {
		SDL_PauseAudioStreamDevice(stream);
		SDL_ClearAudioStream(stream);
		playing = false;
		paused = false;
	}
}

void Sound::pause() {
	if (playing and !paused) {
		SDL_PauseAudioStreamDevice(stream);
		paused = true;
	}
}

void Sound::resume() {
	if (playing and paused) {
		SDL_ResumeAudioStreamDevice(stream);
		paused = false;
	}
}