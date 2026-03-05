#include <SDL3_image/SDL_image.h>
#include "Background.hpp"
#include "Common.hpp"

Background::Background(SDL_Renderer* renderer, std::string_view path) :
	renderer(renderer) {
	texture = IMG_LoadTexture(renderer, path.data());
	if (!texture) {
		std::cerr << "Failed to load background texture : " << SDL_GetError() << std::endl;
		throw std::runtime_error("background texture loading failed");
	}
	srcrect = { 0.0f, 0.0f, WindowWidth, WindowHeight };
	dstrect = { 0.0f, 0.0f, WindowWidth, WindowHeight };
}

Background::~Background(){
	SDL_DestroyTexture(texture);
}

bool Background::present()
{
	if (!SDL_RenderTexture(renderer, texture, &srcrect, &dstrect)) {
		std::cerr << "Failed to render background texture : " << SDL_GetError() << std::endl;
		throw std::runtime_error("background texture rendering failed");
		return false;
	}
	else return true;
}

void Background::setPosition(float x, float y){
	dstrect.x = x;
	dstrect.y = y;
}
