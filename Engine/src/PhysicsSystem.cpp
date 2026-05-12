#include "PhysicsSystem.hpp"
#include "Entity.hpp"

PhysicsSystem::PhysicsSystem(std::vector<std::unique_ptr<Entity>>* entities) : entities(entities) {
}

bool PhysicsSystem::Overlap(SDL_FRect* rect1, SDL_FRect* rect2) {
	float ax1 = rect1->x;
	float ay1 = rect1->y;
	float ax2 = ax1 + rect1->w;
	float ay2 = ay1 + rect1->h;

	float bx1 = rect2->x;
	float by1 = rect2->y;
	float bx2 = bx1 + rect2->w;
	float by2 = by1 + rect2->h;

	if (ax1 < bx2 &&
		ax2 > bx1 &&
		ay1 < by2 &&
		ay2 > by1)
	return true;
	else return false;
}
