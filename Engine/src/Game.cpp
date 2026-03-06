#include <iostream>
#include <memory>
#include "Game.hpp"
#include "Common.hpp"

Game::Game(){
	std::cout << "Initializing Game...\n";
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		std::cerr << "SDL initialization failed : " << SDL_GetError() << std::endl;
		throw std::runtime_error("SDL_Init failed");
	}
	
	window = SDL_CreateWindow("TIT4N", WindowWidth, WindowHeight, 0);
	if (!window) {
		std::cerr << "Window creation failed : " << SDL_GetError() << std::endl;
		throw std::runtime_error("Window creation failed");
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		std::cerr << "Renderer creation failed : " << SDL_GetError() << std::endl;
		throw std::runtime_error("Renderer creation failed");
	}

	sceneManager = std::make_unique<SceneManager>();
	renderSystem = std::make_unique<RenderSystem>();
	physicsSystem = std::make_unique<PhysicsSystem>();
	luaManager = std::make_unique<LuaManager>();

	ServiceLocater::registerSceneManager(sceneManager.get());
	ServiceLocater::registerRenderSystem(renderSystem.get());
	ServiceLocater::registerPhysicsSystem(physicsSystem.get());
	ServiceLocater::registerLuaManager(luaManager.get());
}

Game::~Game(){
	std::cout << "Cleaning Game...\n";

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::Run(){
	std::cout << "Running Game...\n";

	SDL_Event event;
	running = true;

	while (running) {
		while(SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_EVENT_QUIT:
				running = false;
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);

	}
}
