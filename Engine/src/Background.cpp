#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "Background.hpp"
#include "Game.hpp"
#include "ServiceLocator.hpp"

Background::Background(SDL_Renderer* renderer, std::string_view path) :
	renderer(renderer) {
	texture = IMG_LoadTexture(renderer, path.data());
	if (!texture) {
		std::cerr << "Failed to load background texture : " << SDL_GetError() << std::endl;
		throw std::runtime_error("background texture loading failed");
	}

	int w, h;
	ServiceLocator::getGame()->getWindowSize(&w, &h);
	srcrect = { 0.0f, 0.0f, static_cast<float>(w), static_cast<float>(h) };
	dstrect = { 0.0f, 0.0f, static_cast<float>(w), static_cast<float>(h) };

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
