#include "Sound.hpp"
#include "Common.hpp"

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
	if (!SDL_LoadWAV((getBasePath() + "Game/" + filePath).c_str(), &spec, &buffer, &length)) {
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
	int bytesPerSample = SDL_AUDIO_BYTESIZE(spec.format);
	int bytesPerFrame = bytesPerSample * spec.channels;
	int bytesPerSecond = spec.freq * bytesPerFrame;
	duration = (float)length / (float)bytesPerSecond * 1000.0f;
}

Sound::Sound(std::string filePath, SDL_AudioDeviceID dev)
	: device(dev), buffer(nullptr), stream(nullptr), startTick(0), endTick(0), playing(false), paused(false), usable(true), errorId(0), duration(0.0f), loopStartMs(0), loopEndMs(0)
{
	init(filePath);
}

Sound::~Sound() {
	cleanUp();
}

int Sound::getErrorId() const {
	return errorId;
}

Uint64 Sound::getStartTick() const {
	return startTick;
}

Uint64 Sound::getEndTick() const {
	return endTick;
}

bool Sound::isUsable() const {
	return usable;
}

bool Sound::isPlaying() const {
	return playing;
}
void Sound::play() {
	if (!usable || !buffer || !stream) return;
	play(0, (int)(duration));
}

void Sound::play(int startMs) {
	if (!usable || !buffer || !stream) return;
	play(startMs, (int)(duration));
}

void Sound::play(int startMs, int endMs) {
	if (!usable || !buffer || !stream) return;

	loopStartMs = startMs;
	loopEndMs = endMs;

	int bytesPerSample = SDL_AUDIO_BYTESIZE(spec.format);
	int bytesPerFrame = bytesPerSample * spec.channels;
	int bytesPerSecond = spec.freq * bytesPerFrame;

	Uint32 byteStart = (Uint32)((startMs / 1000.0f) * bytesPerSecond);
	Uint32 byteEnd = (Uint32)((endMs / 1000.0f) * bytesPerSecond);

	byteStart = (byteStart / bytesPerFrame) * bytesPerFrame;
	byteEnd = (byteEnd / bytesPerFrame) * bytesPerFrame;

	byteStart = SDL_min(byteStart, length);
	byteEnd = SDL_min(byteEnd, length);

	// SDL_Log("play() called: startMs=%d endMs=%d byteStart=%u byteEnd=%u length=%u", startMs, endMs, byteStart, byteEnd, length);

	if (byteStart >= byteEnd) {
		SDL_Log("play() aborted: byteStart >= byteEnd");
		return;
	}

	stop();

	if (!SDL_PutAudioStreamData(stream, buffer + byteStart, (int)(byteEnd - byteStart))) {
		SDL_Log("PutAudioStreamData failed: %s", SDL_GetError());
		return;
	}
	if (!SDL_ResumeAudioStreamDevice(stream)) {
		SDL_Log("ResumeAudioStreamDevice failed: %s", SDL_GetError());
		return;
	}

	// SDL_Log("play() success, queued=%d", SDL_GetAudioStreamQueued(stream));
	playing = true;
	paused = false;
	startTick = SDL_GetTicks();
	endTick = 0;
}

void Sound::update() {
	if (playing && !paused) {
		Uint64 elapsed = SDL_GetTicks() - startTick;
		int queued = SDL_GetAudioStreamQueued(stream);
		// SDL_Log("update: elapsed=%llu queued=%d", elapsed, queued);
		if (elapsed > 100 && queued == 0) {
			if (loop) play(loopStartMs, loopEndMs);
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
		endTick = SDL_GetTicks();
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

bool Sound::isLooping() const {
	return loop;
}

void Sound::setLooping(bool loop) {
	loop = loop;
}
