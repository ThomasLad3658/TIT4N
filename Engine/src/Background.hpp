#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "Common.hpp"

class Background {
public:
	Background(SDL_Renderer* renderer, const char* path);
	~Background();
	bool present();

private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	const char* path;
	SDL_FRect srcrect;
	SDL_FRect dstrect;
	float x;
	float y;
};