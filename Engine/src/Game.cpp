#include <iostream>
#include <memory>
#include "Game.hpp"
#include "ServiceLocator.hpp"
#include "SceneManager.hpp"
#include "RenderSystem.hpp"
#include "PhysicsSystem.hpp"
#include "LuaManager.hpp"
#include "Entity.hpp"
#include "Common.hpp"

Game::Game() {
	std::cout << "Initializing Game...\n";
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		std::cerr << "SDL initialization failed : " << SDL_GetError() << std::endl;
		throw std::runtime_error("SDL_Init failed");
	}
	
	window = nullptr;
	windowTitle = "";
	windowWidth = 0;
	windowHeight = 0;

	sceneManager = std::make_unique<SceneManager>(&entities);
	renderSystem = std::make_unique<RenderSystem>(&entities);
	physicsSystem = std::make_unique<PhysicsSystem>();
	luaManager = std::make_unique<LuaManager>();

	ServiceLocator::registerGame(this);
	ServiceLocator::registerSceneManager(sceneManager.get());
	ServiceLocator::registerRenderSystem(renderSystem.get());
	ServiceLocator::registerPhysicsSystem(physicsSystem.get());
	ServiceLocator::registerLuaManager(luaManager.get());

}

Game::~Game() {
	std::cout << "Cleaning Game...\n";

	for (int i = 0; i < entities.size(); i++) {
		delete entities[i];
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::Run() {
	std::cout << "Running Game...\n";

	const Uint64 frameRate = 60;
	const Uint64 frameDelay = 1000000000 / frameRate;

	Uint64 lastTick = SDL_GetTicksNS();
	Uint64 currentTick = 0;
	Uint64 dt;

	Uint64 frameStart;
	Uint64 frameTime;

	luaManager->RegisterFunction(this, &Game::CreateWindow, "CreateWindow");
	luaManager->RegisterFunction(this, &Game::SetWindowTitle, "SetWindowTitle");
	luaManager->RegisterFunction(this, &Game::SetWindowSize, "SetWindowSize");
	luaManager->RegisterFunction(sceneManager.get(), &SceneManager::LoadLevel, "LoadLevel");

	luaManager->DoFile((getBasePath() + "Game/main.lua").c_str());
	if (!window) {
		std::cerr << "Window isn't initialized" << std::endl;
		throw std::runtime_error("Window wasn't created yet");
	}

	SDL_Event event;
	running = true;
	while (running) {
		currentTick = frameStart = SDL_GetTicksNS();
		dt = currentTick - lastTick;
		lastTick = currentTick;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_EVENT_QUIT:
				running = false;
				break;
			}
		}
		float dtSeconds = dt / 1000000000.0f;
		for (auto& entity : entities) {
			entity->Update(dtSeconds);
		}
		renderSystem->render();

		frameTime = SDL_GetTicksNS() - frameStart;
		if (frameDelay > frameTime) {
			
			if (frameDelay - frameTime > 1000000) {
				SDL_Delay((frameDelay - frameTime) / 1000000 - 1);
			}
			while(SDL_GetTicksNS() - frameStart < frameDelay){}
			
			//SDL_Delay((frameDelay - frameTime) / 1000000);
			// Not precise enough
		}
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
	renderSystem->Init(window);
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

bool Game::registerEntity(Entity* entity) {
	// Returns true if the entity has been successfuly registered, false if the entity was already registered
	if (isEntityRegistered(entity)) return false;
	//if (!entity->isInitialized()) entity->Init(renderer); why does that exist ? the entity should be initialized when it's created, not when it's registered to the render system
	entities.push_back(entity);
	return true;
}

bool Game::isEntityRegistered(Entity* entity) {
	for (const auto& e : entities) {
		if (e->getId() == entity->getId()) {
			return true;
		}
	}
	return false;
}

bool Game::unregisterEntity(Entity* entity) {
	// Returns true if the entity was found and removed, false otherwise
	for (auto i = entities.begin(); i != entities.end(); ++i) {
		if ((*i)->getId() == entity->getId()) {
			entities.erase(i);
			return true;
		}
	}
	return false;
}