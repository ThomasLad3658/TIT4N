#pragma once
#include <SDL3/SDL.h>

enum SoundErrorId {
	LOAD_WAV = 1,
};

class Sound
{
public:
	Sound(SoundErrorId errId);
	Sound(SDL_AudioSpec spec, Uint8* buf, Uint32 len, SDL_AudioStream* s, SDL_AudioDeviceID dev);
	~Sound();
	int getErrorId() const;
	int getStartTick() const;
	bool isUsable() const;
	bool isPlaying() const;
	void play();
	void update();
	void stop();
private:
	SDL_AudioSpec audioSpec;
	Uint8* buffer;
	Uint32 length;
	SDL_AudioStream* stream;
	SDL_AudioDeviceID device;
	int startTick;
	bool playing;
	bool usable;
	int errorId;
};