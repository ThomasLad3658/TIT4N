#include "RenderSystem.hpp"
#include <map>

RenderSystem::RenderSystem(std::vector<std::unique_ptr<Entity>>* entities) : entities(entities){
	renderer = nullptr;
}

RenderSystem::~RenderSystem() {
	SDL_DestroyRenderer(renderer);
}

void RenderSystem::Init(SDL_Window* window) {
	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		std::cerr << "Renderer creation failed : " << SDL_GetError() << std::endl;
		throw std::runtime_error("Renderer creation failed");
	}
	SDL_SetDefaultTextureScaleMode(renderer, SDL_SCALEMODE_PIXELART);
}

bool RenderSystem::render() {
	std::map<unsigned char, std::vector<Entity*>> renderLayers = {};
	for (unsigned int i = 0; i < entities->size(); i++) {
		unsigned char z = (*entities)[i]->getRenderLayer();
		renderLayers[z].push_back((*entities)[i].get());
	}

	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	for (unsigned char i = 0; i < 255; i++) {
		if (renderLayers.find(i) != renderLayers.end()) {
			for (unsigned int j = 0; j < renderLayers[i].size(); j++) {
				renderLayers[i][j]->present();
			}
		}
	}
	if (renderLayers.find(255) != renderLayers.end()) {
		for (unsigned int j = 0; j < renderLayers[255].size(); j++) {
			renderLayers[255][j]->present();
		}
	}

	SDL_RenderPresent(renderer);
	return true;
}

SDL_Renderer* RenderSystem::getRenderer() {
	return renderer;
}