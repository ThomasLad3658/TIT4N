#pragma once
#include <iostream>
#include <SDL3/SDL.h>

std::string getBasePath();

struct CollisionInfo {
	bool isColliding;
	SDL_FRect overlapRect;
};