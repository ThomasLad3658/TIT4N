#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <memory>
#include "Background.hpp"
#include "Common.hpp"

class Game {
public:
	Game();
	~Game();
	void Run();
private:
	bool running = false;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	std::unique_ptr<Background> background;
};