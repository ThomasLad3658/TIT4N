#include "Background.hpp"

Background::Background(SDL_Renderer* renderer, const char* path) : 
	renderer(renderer), path(path) {
	texture = IMG_LoadTexture(renderer, path);
	if (!texture) {
		std::cerr << "Failed to load background texture : " << SDL_GetError() << std::endl;
	}
	srcrect = { 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT };
	dstrect = { 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT };
}

Background::~Background(){
	SDL_DestroyTexture(texture);
}

bool Background::present()
{
	if (!SDL_RenderTexture(renderer, texture, &srcrect, &dstrect)) {
		std::cerr << "Failed to render background texture : " << SDL_GetError() << std::endl;
		return false;
	}
	else return true;
}

void Background::setPosition(float x, float y){
	dstrect.x = x;
	dstrect.y = y;
}
