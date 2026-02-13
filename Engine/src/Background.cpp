#include "Background.hpp"

Background::Background(SDL_Renderer* renderer, const char* path) : renderer(renderer), path(path) {
	texture = IMG_LoadTexture(renderer, path);
	if (!texture) {
		std::cerr << "Failed to load background texture : " << SDL_GetError() << std::endl;
	}
	w = 
}

Background::~Background(){
	SDL_DestroyTexture(texture);
}
