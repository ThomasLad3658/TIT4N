#include "Common.hpp"

std::string getBasePath() {
	const char* basePath = SDL_GetBasePath();
	if (!basePath) {
		throw std::runtime_error("SDL_GetBasePath() failed");
	}
	return basePath;
}