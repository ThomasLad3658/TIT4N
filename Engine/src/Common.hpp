#pragma once
#include <iostream>
#include <SDL3/SDL.h>

class Entity;

std::string getBasePath();

struct CollisionInfo {
	bool isColliding;
	SDL_FRect overlapRect;
	Entity* entity1;
	Entity* entity2;
};