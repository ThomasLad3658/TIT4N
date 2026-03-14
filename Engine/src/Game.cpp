#include <iostream>
#include <memory>
#include "Game.hpp"
#include "ServiceLocator.hpp"
#include "SceneManager.hpp"
#include "RenderSystem.hpp"
#include "PhysicsSystem.hpp"
#include "LuaManager.hpp"
#include "Entity.hpp"

Game::Game(){
	std::cout << "Initializing Game...\n";
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		std::cerr << "SDL initialization failed : " << SDL_GetError() << std::endl;
		throw std::runtime_error("SDL_Init failed");
	}
	
	window = nullptr;
	renderer = nullptr;
	windowTitle = "";
	windowWidth = 0;
	windowHeight = 0;

	sceneManager = std::make_unique<SceneManager>();
	renderSystem = std::make_unique<RenderSystem>();
	physicsSystem = std::make_unique<PhysicsSystem>();
	luaManager = std::make_unique<LuaManager>();

	ServiceLocator::registerGame(this);
	ServiceLocator::registerSceneManager(sceneManager.get());
	ServiceLocator::registerRenderSystem(renderSystem.get());
	ServiceLocator::registerPhysicsSystem(physicsSystem.get());
	ServiceLocator::registerLuaManager(luaManager.get());

}

Game::~Game(){
	std::cout << "Cleaning Game...\n";

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::Run(){
	std::cout << "Running Game...\n";

	luaManager->RegisterFunction(this, &Game::CreateWindow, "CreateWindow");
	luaManager->RegisterFunction(this, &Game::SetWindowTitle, "SetWindowTitle");
	luaManager->RegisterFunction(this, &Game::SetWindowSize, "SetWindowSize");

	const char* basePath = SDL_GetBasePath();
	if (!basePath) {
		throw std::runtime_error("SDL_GetBasePath() failed");
	}
	luaManager->DoFile((std::string(basePath) + "Game/main.lua").c_str());

	SDL_Event event;
	running = true;

	// ******************** TEST ********************

	std::string path = std::string(basePath) + "Game/assets/sprites/player/Soldier.png";
	SDL_FRect src = { 0.0f, 0.0f, 100.0f, 100.0f };
	SDL_FRect dst = { 0.0f, 0.0f, 100.0f, 100.0f };
	Entity* entity = new Entity(renderer, path, src, dst);

	// ******************** TEST ********************

	while (running) {
		while(SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_EVENT_QUIT:
				running = false;
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// ******************** TEST ********************

		entity->present();

		// ******************** TEST ********************

		SDL_RenderPresent(renderer);

	}
}

void Game::CreateWindow(const char* title, int width, int height) {
	windowTitle = title;
	windowWidth = width;
	windowHeight = height;
	window = SDL_CreateWindow(title, width, height, 0);
	if (!window) {
		std::cerr << "Window creation failed : " << SDL_GetError() << std::endl;
		throw std::runtime_error("Window creation failed");
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		std::cerr << "Renderer creation failed : " << SDL_GetError() << std::endl;
		throw std::runtime_error("Renderer creation failed");
	}
	
	SDL_SetDefaultTextureScaleMode(renderer, SDL_SCALEMODE_PIXELART);
}

bool Game::SetWindowTitle(const char* title) {
	return SDL_SetWindowTitle(window, title);
}

bool Game::SetWindowSize(int w, int h) {
	return SDL_SetWindowSize(window, w, h);
}

const char* Game::getWindowTitle() {
	return windowTitle;
}

void Game::getWindowSize(int* width, int* height) {
	*width = windowWidth;
	*height = windowHeight;
}
