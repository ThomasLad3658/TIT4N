#pragma once
#include <SDL3/SDL.h>
#include <iostream>

class Entity {
public:
	Entity(SDL_Renderer* renderer, std::string_view path, SDL_FRect* src, SDL_FRect* dst);
	~Entity();
	bool present();
	void setPosition(float x, float y);
	SDL_FRect getDstRect();
	std::string getTag();
	unsigned char getRenderLayer();

protected:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_FRect srcrect;
	SDL_FRect dstrect;
	std::string tag;
	unsigned char renderLayer;
	bool isStatic;
	bool hasScript;
};