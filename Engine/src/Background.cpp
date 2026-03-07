#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "Background.hpp"
#include "Common.hpp"

Background::Background(SDL_Renderer* renderer, std::string_view path) :
	renderer(renderer) {
	texture = IMG_LoadTexture(renderer, path.data());
	if (!texture) {
		std::cerr << "Failed to load background texture : " << SDL_GetError() << std::endl;
		throw std::runtime_error("background texture loading failed");
	}
	srcrect = { 0.0f, 0.0f, kWindowWidth, kWindowHeight };
	dstrect = { 0.0f, 0.0f, kWindowWidth, kWindowHeight };
}

Background::~Background(){
	SDL_DestroyTexture(texture);
}

void Background::present()
{
	if (!SDL_RenderTexture(renderer, texture, &srcrect, &dstrect)) {
		std::cerr << "Failed to render background texture : " << SDL_GetError() << std::endl;
		throw std::runtime_error("background texture rendering failed");
	}
}

void Background::setPosition(float x, float y){
	dstrect.x = x;
	dstrect.y = y;
}
