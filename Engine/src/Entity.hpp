#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <string_view>

class Entity {
public:
	Entity(
		std::string tag, 
		int refenceIndex,
		std::string filepath, 
		const SDL_FRect& src, 
		const SDL_FRect& dst
	);
	~Entity();
	void Init(SDL_Renderer* sdlRenderer);
	void destroy();
	bool present();
	void Update(float dt);
	bool isInitialized() const;
	void setPosition(float x, float y);
	void setRenderLayer(unsigned char z);
	SDL_FRect getDstRect() const;
	std::string getTag() const;
	unsigned char getRenderLayer() const;
	unsigned int getId() const;

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
	unsigned int id;
	int referenceIndex;
	static inline unsigned int nextId = 0;
};