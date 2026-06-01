#pragma once
#include "LuaManager.hpp"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <memory>
#include <vector>

class Game;
class SceneManager;
class RenderSystem;
class PhysicsSystem;
class LuaManager;
class Entity;
class InputManager;
class SoundSystem;

class Game {
public:
	Game();
	~Game();
	void Run();
	void CreateWindow(const char* title, int width, int height);
	void Quit();
	bool SetWindowFullscreen(bool fullscreenValue);
	bool SetWindowBorderless(bool borderlessValue);
	bool SetWindowTitle(const char* title);
	bool SetWindowSize(int w, int h);
	void SetFrameRate(int fps);
	std::string getWindowTitle();
	void getWindowSize(int* width, int* height);
	std::unique_ptr<Entity> CreateEntity(std::string dataPath);
	void CreateEntityFromLua(std::string entityPath);
	bool DeleteEntity(Entity* entity);
	bool registerEntity(std::unique_ptr<Entity> entity);
	bool isEntityRegistered(Entity* entity);
	bool unregisterEntity(Entity* entity);
	int GetEntityIdByName(std::string name);
	bool IsEntityAlive(int id);
	template<typename T>
	T GetEntityVariable(unsigned int id, std::string variablePath);

private:
	bool running = false;
	bool fullscreen = false;
	bool borderless = false;
	SDL_Window* window;
	std::unique_ptr<SceneManager> sceneManager;
	std::unique_ptr<RenderSystem> renderSystem;
	std::unique_ptr<SoundSystem> soundSystem;
	std::unique_ptr<PhysicsSystem> physicsSystem;
	std::unique_ptr<LuaManager> luaManager;
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<Entity>> creationQueue;
	std::vector<Entity*> deletionQueue;
	std::unique_ptr<InputManager> inputManager;
	std::string windowTitle;
	int windowWidth;
	int windowHeight;
	Uint64 frameRate;
	Uint64 frameDelay;
};

template <typename T>
T Game::GetEntityVariable(unsigned int id, std::string variablePath) {
	for (const auto& entity : entities) {
		if (entity->getId() == id) {
			return luaManager->GetVariable<T>(("/" + std::to_string(entity->getReferenceIndex()) + "." + variablePath).c_str());
		}
	}
	std::cerr << "Couldn't find entity with id : " << id << std::endl;
	throw std::runtime_error("Couldn't find entity with id");
}