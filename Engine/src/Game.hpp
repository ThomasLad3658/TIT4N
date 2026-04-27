#pragma once
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

class Game {
public:
	Game();
	~Game();
	void Run();
	void CreateWindow(const char* title, int width, int height);
	bool SetWindowTitle(const char* title);
	bool SetWindowSize(int w, int h);
	void SetFrameRate(int fps);
	std::string getWindowTitle();
	void getWindowSize(int* width, int* height);
	std::unique_ptr<Entity> CreateEntity(std::string dataPath);
	bool registerEntity(std::unique_ptr<Entity> entity);
	bool isEntityRegistered(Entity* entity);
	bool unregisterEntity(Entity* entity);

private:
	bool running = false;
	SDL_Window* window;
	std::unique_ptr<SceneManager> sceneManager;
	std::unique_ptr<RenderSystem> renderSystem;
	std::unique_ptr<PhysicsSystem> physicsSystem;
	std::unique_ptr<LuaManager> luaManager;
	std::vector<std::unique_ptr<Entity>> entities;
	std::string windowTitle;
	int windowWidth;
	int windowHeight;
	Uint64 frameRate;
	Uint64 frameDelay;
};