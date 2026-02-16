#include "Game.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

Game::Game(){
	std::cout << "Initializing Game...\n";
	if (SDL_Init(SDL_INIT_VIDEO || SDL_INIT_AUDIO) == 0) {
		std::cerr << "SDL initialization failed : " << SDL_GetError() << std::endl;
	}

	window = SDL_CreateWindow("TIT4N", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (!window) {
		std::cerr << "Window creation failed : " << SDL_GetError() << std::endl;
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		std::cerr << "Renderer creation failed : " << SDL_GetError() << std::endl;
	}
}

Game::~Game(){
	std::cout << "Cleaning Game...";

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::Run(){
	std::cout << "Running Game...\n";

	running = true;

	while (running) {
		while(SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_EVENT_QUIT:
				running = false;
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);

	}
}
