#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <memory>

class Entity;

class PhysicsSystem{
public:
	PhysicsSystem(std::vector<std::unique_ptr<Entity>>* entities);
private:
	std::vector<std::unique_ptr<Entity>>* entities;
	bool Overlap(SDL_FRect* rect1, SDL_FRect* rect2);
};

