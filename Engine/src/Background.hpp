#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>

class Background {
public:
	Background(SDL_Renderer* renderer, const char* path);
	~Background();

private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	const char* path;
	uint16_t w;
	uint16_t h;
};