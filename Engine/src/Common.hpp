#pragma once
#include <iostream>
#include <SDL3/SDL.h>

template <typename T>
struct structVariable {
	const char* name;
	T* var;
};

std::string getBasePath();