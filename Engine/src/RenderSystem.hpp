#pragma once
#include <SDL3/SDL.h>
#include "Entity.hpp"
#include <vector>

class RenderSystem {
public:
	RenderSystem(std::vector<Entity*>*);
	~RenderSystem();
	void Init(SDL_Window* window);
	bool render();
	SDL_Renderer* getRenderer();
private:
	SDL_Renderer* renderer;
	std::vector<Entity*>* entities;
};