#pragma once
#include <SDL3/SDL.h>
#include "Entity.hpp"
#include <memory>
#include <vector>

class RenderSystem {
public:
	RenderSystem(std::vector<std::unique_ptr<Entity>>*);
	~RenderSystem();
	void Init(SDL_Window* window);
	bool render();
	SDL_Renderer* getRenderer();
private:
	SDL_Renderer* renderer;
	std::vector<std::unique_ptr<Entity>>* entities;
};