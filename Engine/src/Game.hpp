#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <memory>

class Game;
class SceneManager;
class RenderSystem;
class PhysicsSystem;
class LuaManager;
class SoundSystem;

class Game {
public:
	Game();
	~Game();
	void Run();
	void CreateWindow(const char* title, int width, int height);
	bool SetWindowTitle(const char* title);
	bool SetWindowSize(int w, int h);
	const char* getWindowTitle();
	void getWindowSize(int* width, int* height);

private:
	bool running = false;
	SDL_Window* window;
	std::unique_ptr<SceneManager> sceneManager;
	std::unique_ptr<RenderSystem> renderSystem;
	std::unique_ptr<SoundSystem> soundSystem;
	std::unique_ptr<PhysicsSystem> physicsSystem;
	std::unique_ptr<LuaManager> luaManager;
	const char* windowTitle;
	int windowWidth;
	int windowHeight;
};