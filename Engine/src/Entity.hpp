#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <string_view>

class Entity {
public:
	Entity(SDL_Renderer* renderer, std::string_view path, const SDL_FRect& src, const SDL_FRect& dst);
	~Entity();
	void present();
	void setPosition(float x, float y);
	SDL_FRect getDstRect() const;
	std::string getTag() const;
	unsigned char getRenderLayer() const;

protected:
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_FRect srcrect;
	SDL_FRect dstrect;
	SDL_FRect hitbox;
	std::string tag;
	unsigned char renderLayer;
	bool isStatic;
	bool hasScript;
};