#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <string_view>

class Entity {
public:
	Entity(std::string filepath, const SDL_FRect& src, const SDL_FRect& dst);
	~Entity();
	void Init(SDL_Renderer* sdlRenderer);
	bool present();
	bool isInitialized() const;
	void setPosition(float x, float y);
	void setRenderLayer(char z);
	SDL_FRect getDstRect() const;
	std::string getTag() const;
	unsigned char getRenderLayer() const;
	unsigned int getId();

protected:
	bool initialized = false;
	std::string filepath;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_FRect srcrect;
	SDL_FRect dstrect;
	SDL_FRect hitbox;
	std::string tag;
	unsigned char renderLayer;
	bool isStatic;
	bool hasScript;
	unsigned int id;
	static inline unsigned int nextId = 0;
};