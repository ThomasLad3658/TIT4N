#include "RenderSystem.hpp"

RenderSystem::RenderSystem(SDL_Renderer* sdlRenderer)
{
	renderer = sdlRenderer;
}

RenderSystem::~RenderSystem() {

}

void RenderSystem::destroy() {
	entities.clear();
	SDL_DestroyRenderer(renderer);
}

bool RenderSystem::render() {
	std::map<unsigned char, std::vector<Entity*>> renderLayers = {};
	for (unsigned int i = 0; i < entities.size(); i++) {
		unsigned char z = entities[i]->getRenderLayer();
		renderLayers[z].push_back(entities[i]);
	}

	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	for (unsigned char i = 0; i < 128; i++) {
		if (renderLayers.find(i) != renderLayers.end()) {
			for (unsigned int j = 0; i < renderLayers[i].size(); j++) {
				renderLayers[i][j]->present();
			}
		}
	}

	SDL_RenderPresent(renderer);
	return true;
}

bool RenderSystem::registerEntity(Entity* entity) {
	// Returns true if the entity is has been successfuly registred, false if the entity was already registered
	if (isEntityRegistered(entity)) return false;
	entity->getRenderLayer();
	entities.push_back(entity);
	return true;
}

bool RenderSystem::isEntityRegistered(Entity* entity) {
	// Returns true if the entity is found, false otherwise
	auto it = std::find(entities.begin(), entities.end(), entity);
	return it != entities.end();
}

bool RenderSystem::unregisterEntity(Entity* entity) {
	// Returns true if the entity was found and removed, false otherwise
	auto it = std::find(entities.begin(), entities.end(), entity);
	if (it == entities.end()) return false;
	entities.erase(it);
	return true;
}