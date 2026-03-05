#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include "Background.hpp"

class Game {
public:
	Game();
	~Game();
	void Run();
private:
	bool running = false;
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::unique_ptr<Background> background;
};