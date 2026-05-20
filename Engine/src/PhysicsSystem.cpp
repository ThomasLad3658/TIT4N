#include "PhysicsSystem.hpp"
#include "Entity.hpp"

PhysicsSystem::PhysicsSystem(std::vector<std::unique_ptr<Entity>>* entities) : entities(entities) 
{}

void PhysicsSystem::CheckCollisions() {
	collisions.clear();
	for (int i = 0; i < entities->size(); i++) {
		for (int j = i + 1; j < entities->size(); j++) {

			SDL_FRect rect1 = (*entities)[i].get()->getHitbox();
			SDL_FRect rect2 = (*entities)[j].get()->getHitbox();

			if (isOverlap(&rect1, &rect2)) {
				collisions.push_back({
					true,
					getOverlap(&rect1, &rect2),
					(*entities)[i].get(),
					(*entities)[j].get()
				});
			}
			else {
				collisions.push_back({
					false,
					SDL_FRect(),
					(*entities)[i].get(),
					(*entities)[j].get()
				});
			}
		}
	}
}

void PhysicsSystem::Update() {
	CheckCollisions();
	for (const auto& collision : collisions) {
		if (collision.isColliding) {
			collision.entity1->Collisions(collision.entity2->getTag(), collision.entity2->getId(), collision.overlapRect);
			collision.entity2->Collisions(collision.entity1->getTag(), collision.entity1->getId(), collision.overlapRect);
		}
	}
}

bool PhysicsSystem::isOverlap(SDL_FRect* rect1, SDL_FRect* rect2) {
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

SDL_FRect PhysicsSystem::getOverlap(SDL_FRect* rect1, SDL_FRect* rect2)
{
	const float x1 = std::max(rect1->x, rect2->x);
	const float y1 = std::max(rect1->y, rect2->y);
	const float x2 = std::min(rect1->x + rect1->w, rect2->x + rect2->w);
	const float y2 = std::min(rect1->y + rect1->h, rect2->y + rect2->h);

	const float w = x2 - x1;
	const float h = y2 - y1;

	return { x1, y1, w, h};
}