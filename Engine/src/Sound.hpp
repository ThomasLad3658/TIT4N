#pragma once
#include <SDL3/SDL.h>
#include <string>

enum SoundErrorId {
	LOAD_WAV,
	LOAD_DEVICE
};

class Sound
{
public:
	Sound(std::string filePath, SDL_AudioDeviceID dev);
	~Sound();
	int getErrorId() const;
	Uint64 getStartTick() const;
	Uint64 getEndTick() const;
	bool isUsable() const;
	bool isPlaying() const;
	void play();
	void play(int startMs);
	void play(int startMs, int endMs);
	void update();
	void stop();
	void pause();
	void resume();
	bool isLooping() const;
	void setLooping(bool loop);

private:
	void cleanUp();
	void init(std::string filePath);
	SDL_AudioSpec spec;
	Uint8* buffer;
	Uint32 length;
	SDL_AudioStream* stream;
	SDL_AudioDeviceID device;
	Uint64 startTick;
	Uint64 endTick;
	bool playing;
	bool paused;
	bool usable;
	bool loop;
	int errorId;
	float duration;
	int loopStartMs;
	int loopEndMs;
};