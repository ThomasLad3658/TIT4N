#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>

class Game {
public:
	Game();
	~Game();
	void Run();
private:
	bool running;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
};