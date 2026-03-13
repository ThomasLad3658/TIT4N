#pragma once
#include <SDL3/SDL.h>
#include <string_view>

class Background {
public:
	Background(SDL_Renderer* renderer, std::string_view path);
	~Background();
	void present();
	void setPosition(float x, float y);

private:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_FRect srcrect;
	SDL_FRect dstrect;
};