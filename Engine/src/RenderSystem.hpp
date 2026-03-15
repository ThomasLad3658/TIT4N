#pragma once
#include <SDL3/SDL.h>
#include "Entity.hpp"
#include <vector>
#include <map>

class RenderSystem {
public:
	RenderSystem();
	~RenderSystem();
	void Init(SDL_Window* window);
	bool render();
	bool registerEntity(Entity* entity);
	bool isEntityRegistered(Entity* entity);
	bool unregisterEntity(Entity* entity);
private:
	SDL_Renderer* renderer;
	std::vector<Entity*> entities;
};