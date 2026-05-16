#include <iostream>
#include <memory>
#include "Game.hpp"
#include "ServiceLocator.hpp"
#include "SceneManager.hpp"
#include "RenderSystem.hpp"
#include "PhysicsSystem.hpp"
#include "LuaManager.hpp"
#include "InputManager.hpp"
#include "Entity.hpp"
#include "Common.hpp"
#include "SoundSystem.hpp"

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

	frameRate = 60;
	frameDelay = 1000000000 / frameRate;

	sceneManager = std::make_unique<SceneManager>(&entities);
	renderSystem = std::make_unique<RenderSystem>(&entities);
	soundSystem = std::make_unique<SoundSystem>();
	physicsSystem = std::make_unique<PhysicsSystem>(&entities);
	luaManager = std::make_unique<LuaManager>();
	inputManager = std::make_unique<InputManager>();

	ServiceLocator::registerGame(this);
	ServiceLocator::registerSceneManager(sceneManager.get());
	ServiceLocator::registerRenderSystem(renderSystem.get());
	ServiceLocator::registerSoundSystem(soundSystem.get());
	ServiceLocator::registerPhysicsSystem(physicsSystem.get());
	ServiceLocator::registerLuaManager(luaManager.get());
	ServiceLocator::registerInputManager(inputManager.get());

}

Game::~Game() {
	std::cout << "Cleaning Game...\n";
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::Run() {
	std::cout << "Running Game...\n";
	
	Uint64 lastTick = SDL_GetTicksNS();
	Uint64 currentTick = 0;
	Uint64 dt;
	
	Uint64 frameStart;
	Uint64 frameTime;

	luaManager->RegisterFunction(this, &Game::CreateWindow, "CreateWindow");
	luaManager->RegisterFunction(this, &Game::SetWindowTitle, "SetWindowTitle");
	luaManager->RegisterFunction(this, &Game::SetWindowSize, "SetWindowSize");
	luaManager->RegisterFunction(this, &Game::SetFrameRate, "SetFrameRate");
	luaManager->RegisterFunction(sceneManager.get(), &SceneManager::LoadScene, "LoadScene");
	luaManager->RegisterFunction(inputManager.get(), &InputManager::GetKeyState, "GetKeyState");
	luaManager->RegisterFunction(inputManager.get(), &InputManager::GetMouseState, "GetMouseState");
	luaManager->RegisterFunction(soundSystem.get(), &SoundSystem::createSound, "CreateSound");
	luaManager->RegisterFunction(soundSystem.get(), &SoundSystem::play, "PlaySound");
	luaManager->RegisterFunction(soundSystem.get(), &SoundSystem::playFrom, "PlaySoundFrom");
	luaManager->RegisterFunction(soundSystem.get(), &SoundSystem::playFromTo, "PlaySoundFromTo");
	luaManager->RegisterFunction(soundSystem.get(), &SoundSystem::isPlaying, "IsSoundPlaying");
	luaManager->RegisterFunction(soundSystem.get(), &SoundSystem::stop, "StopSound");
	luaManager->RegisterFunction(soundSystem.get(), &SoundSystem::resume, "ResumeSound");
	luaManager->RegisterFunction(soundSystem.get(), &SoundSystem::pause, "PauseSound");
	luaManager->RegisterFunction(soundSystem.get(), &SoundSystem::loop, "LoopSound");


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
			inputManager->ProcessInputEvent(event);
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

		physicsSystem->Update();

		renderSystem->render();

		inputManager->EndOfFrame();

		frameTime = SDL_GetTicksNS() - frameStart;
		if (frameDelay > frameTime) {
			
			if (frameDelay - frameTime > 1000000) {
				SDL_Delay((frameDelay - frameTime) / 1000000 - 1);
			}
			while(SDL_GetTicksNS() - frameStart < frameDelay){}
			
			// Not precise enough
			//SDL_Delay((frameDelay - frameTime) / 1000000);
		}
		soundSystem->update();
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

void Game::SetFrameRate(int fps) {
	frameRate = fps;
	frameDelay = 1000000000 / frameRate;
}

std::string Game::getWindowTitle() {
	return windowTitle;
}

void Game::getWindowSize(int* width, int* height) {
	*width = windowWidth;
	*height = windowHeight;
}

std::unique_ptr<Entity> Game::CreateEntity(std::string dataPath) {
	
	LuaManager* luaManager = ServiceLocator::getLuaManager();
	int ref;

	std::string tag = luaManager->GetVariable<std::string>((dataPath + ".tag").c_str());
	luaManager->DoFile((getBasePath() + "Game/scripts/" + tag + ".lua").c_str());
	ref = luaManager->ReferenceNewObjWithPath(tag.c_str(), dataPath.c_str());
	std::string objPath = "/" + std::to_string(ref);
	float dstScale = luaManager->GetVariable<float>((objPath + ".dstScale").c_str());
	float w = luaManager->GetVariable<float>((objPath + ".srcrect.w").c_str());
	float h = luaManager->GetVariable<float>((objPath + ".srcrect.h").c_str());
	std::unique_ptr<Entity> entity = std::make_unique<Entity>(
		tag,
		ref,
		luaManager->GetVariable<std::string>((objPath + ".path").c_str()),
		SDL_FRect{
			luaManager->GetVariable<float>((objPath + ".srcrect.x").c_str()),
			luaManager->GetVariable<float>((objPath + ".srcrect.y").c_str()),
			w,
			h
		},
		SDL_FRect{
			luaManager->GetVariable<float>((objPath + ".x").c_str()),
			luaManager->GetVariable<float>((objPath + ".y").c_str()),
			dstScale * w,
			dstScale * h
		}
	);
	luaManager->RegisterFunctionToLuaField(entity.get(), &Entity::PlayAnimation, objPath.c_str(), "Play");
	luaManager->RegisterFunctionToLuaField(entity.get(), &Entity::destroy, objPath.c_str(), "Suicide");
	return std::move(entity);

}

bool Game::DeleteEntity(Entity* entity) {
	if (!isEntityRegistered(entity)) return false;
	return unregisterEntity(entity);
}

bool Game::registerEntity(std::unique_ptr<Entity> entity) {
	// Returns true if the entity has been successfuly registered, false if the entity was already registered
	if (isEntityRegistered(entity.get())) return false;
	entities.push_back(std::move(entity));
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