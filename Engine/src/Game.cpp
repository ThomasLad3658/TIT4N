#include <iostream>
#include <memory>
#include "Game.hpp"
#include "Common.hpp"

static bool Lua_SetWindowTitle(const char* title) {
	return ServiceLocator::getGame()->SetWindowTitle(title);
}

static bool Lua_SetWindowSize(int w, int h) {
	return ServiceLocator::getGame()->SetWindowSize(w, h);
}

Game::Game(){
	std::cout << "Initializing Game...\n";
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		std::cerr << "SDL initialization failed : " << SDL_GetError() << std::endl;
		throw std::runtime_error("SDL_Init failed");
	}
	
	window = SDL_CreateWindow("", kWindowWidth, kWindowHeight, 0);
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

	ServiceLocator::registerGame(this);
	ServiceLocator::registerSceneManager(sceneManager.get());
	ServiceLocator::registerRenderSystem(renderSystem.get());
	ServiceLocator::registerPhysicsSystem(physicsSystem.get());
	ServiceLocator::registerLuaManager(luaManager.get());

	luaManager->RegisterFunction(&Lua_SetWindowTitle, "SetWindowTitle");
	luaManager->RegisterFunction(&Lua_SetWindowSize, "SetWindowSize");

	const char* basePath = SDL_GetBasePath();
	if (!basePath) {
		throw std::runtime_error("SDL_GetBasePath() failed");
	}
	luaManager->DoFile((std::string(basePath) + "Game/main.lua").c_str());

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

bool Game::SetWindowTitle(const char* title) {
	return SDL_SetWindowTitle(window, title);
}

bool Game::SetWindowSize(int w, int h) {
	return SDL_SetWindowSize(window, w, h);
}
