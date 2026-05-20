#pragma once
#include "Common.hpp"
#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <memory>

class Entity;

class PhysicsSystem{
public:
	PhysicsSystem(std::vector<std::unique_ptr<Entity>>* entities);
	void CheckCollisions();
	void Update();
private:
	std::vector<std::unique_ptr<Entity>>* entities;
	std::vector<CollisionInfo> collisions;
	bool isOverlap(SDL_FRect* rect1, SDL_FRect* rect2);
	SDL_FRect getOverlap(SDL_FRect* rect1, SDL_FRect* rect2);
};

