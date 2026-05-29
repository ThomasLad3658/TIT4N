#include <iostream>
#include <memory>
#include "Game.hpp"
#include "ServiceLocator.hpp"
#include "SceneManager.hpp"
#include "RenderSystem.hpp"
#include "PhysicsSystem.hpp"
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
	luaManager->RegisterFunction(this, &Game::Quit, "Quit");
	luaManager->RegisterFunction(this, &Game::SetWindowFullscreen, "SetWindowFullscreen");
	luaManager->RegisterFunction(this, &Game::SetWindowBorderless, "SetWindowBorderless");
	luaManager->RegisterFunction(this, &Game::SetWindowTitle, "SetWindowTitle");
	luaManager->RegisterFunction(this, &Game::SetWindowSize, "SetWindowSize");
	luaManager->RegisterFunction(this, &Game::SetFrameRate, "SetFrameRate");
	luaManager->RegisterFunction(this, &Game::CreateEntityFromLua, "CreateEntity");
	luaManager->RegisterFunction(this, &Game::FindEntitybyName, "FindEntitybyName");
	luaManager->RegisterFunction(this, &Game::IsEntityAlive, "IsEntityAlive");
	luaManager->RegisterFunction(this, &Game::GetEntityVariable<int>, "GetEntityInt");
	luaManager->RegisterFunction(this, &Game::GetEntityVariable<float>, "GetEntityFloat");
	luaManager->RegisterFunction(this, &Game::GetEntityVariable<bool>, "GetEntityBool");
	luaManager->RegisterFunction(this, &Game::GetEntityVariable<std::string>, "GetEntityString");
	luaManager->RegisterFunction(sceneManager.get(), &SceneManager::QueueScene, "LoadScene");
	luaManager->RegisterFunction(inputManager.get(), &InputManager::GetKeyState, "GetKeyState");
	luaManager->RegisterFunction(inputManager.get(), &InputManager::GetMouseState, "GetMouseState");
	luaManager->RegisterFunction(soundSystem.get(), &SoundSystem::createSound, "CreateSound");
	luaManager->RegisterFunction(soundSystem.get(), &SoundSystem::destroySound, "DestroySound");
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

		// Call all OnUpdate functions in Lua
		luaManager->callFunction<void>("OnUpdate", false, dtSeconds);
		for (auto& entity : entities) {
			entity->Update(dtSeconds);
		}

		physicsSystem->Update();
		
		renderSystem->render();

		soundSystem->update();

		sceneManager->Update();

		for (int i = 0; i < creationQueue.size(); i++) {
			registerEntity(std::move(creationQueue[i]));
		}

		for (auto& entity : deletionQueue) {
			unregisterEntity(entity);
		}
		
		creationQueue.clear();
		deletionQueue.clear();
		
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

void Game::Quit() {
	running = false;
}

bool Game::SetWindowFullscreen(bool fullscreenValue) {
	fullscreen = SDL_SetWindowFullscreen(window, fullscreenValue);
	return fullscreen;
}

bool Game::SetWindowBorderless(bool borderlessValue) {
	borderless = !SDL_SetWindowBordered(window, !borderlessValue);
	return borderless;
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
	if (luaManager->TryFile((getBasePath() + "Game/scripts/" + tag + ".lua").c_str())) {
		luaManager->DoFile((getBasePath() + "Game/scripts/" + tag + ".lua").c_str());
		ref = luaManager->ReferenceNewObjWithPath(tag.c_str(), dataPath.c_str());
	}
	else {
		ref = luaManager->ReferenceNewObj(dataPath.c_str());
	}
	
	std::string objPath = "/" + std::to_string(ref);
	std::unique_ptr<Entity> entity = std::make_unique<Entity>(ref);
	luaManager->RegisterFunctionToLuaField(entity.get(), &Entity::PlayAnimation, objPath.c_str(), "Play");
	luaManager->RegisterFunctionToLuaField(entity.get(), &Entity::destroy, objPath.c_str(), "Suicide");
	luaManager->RegisterFunctionToLuaField(entity.get(), &Entity::CreateEntityFromEntity, objPath.c_str(), "CreateEntityFromEntity");
	luaManager->RegisterFunctionToLuaField(entity.get(), &Entity::getId, objPath.c_str(), "getSelfId");
	return entity;
	
}

void Game::CreateEntityFromLua(std::string entityPath) {
	std::unique_ptr<Entity> entity = CreateEntity(entityPath);
	entity->Init(renderSystem->getRenderer());
	creationQueue.push_back(std::move(entity));
}

bool Game::DeleteEntity(Entity* entity) {
	if (!isEntityRegistered(entity)) return false;
	deletionQueue.push_back(entity);
	return true;
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

int Game::FindEntitybyName(std::string name) {
	for (const auto& e : entities) {
		if (e->getName() == name) {
			return e->getId();
		}
	}
	return -1;
}

bool Game::IsEntityAlive(int id) {
	for (const auto& entity : entities) {
		if (entity->getId() == id) {
			return true;
		}
	}
	return false;
}