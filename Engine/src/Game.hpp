#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include "Background.hpp"
#include "ServiceLocater.hpp"
#include "SceneManager.hpp"
#include "RenderSystem.hpp"
#include "PhysicsSystem.hpp"
#include "LuaManager.hpp"
extern "C" {
#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>
}

class Game {
public:
	Game();
	~Game();
	void Run();
	bool SetWindowTitle(const char* title);
	bool SetWindowSize(int w, int h);
private:
	bool running = false;
	SDL_Window* window;
	int WindowWIDTH = 0;
	int WindowHEIGHT = 0;
	SDL_Renderer* renderer;
	std::unique_ptr<SceneManager> sceneManager;
	std::unique_ptr<RenderSystem> renderSystem;
	std::unique_ptr<PhysicsSystem> physicsSystem;
	std::unique_ptr<LuaManager> luaManager;
};