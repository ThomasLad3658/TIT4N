#pragma once
#include <SDL3/SDL.h>
#include <iostream>

class Background {
public:
	Background(SDL_Renderer* renderer, std::string_view path);
	~Background();
	bool present();
	void setPosition(float x, float y);

private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_FRect srcrect;
	SDL_FRect dstrect;
};