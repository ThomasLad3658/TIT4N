#pragma once
#include <iostream>
#include "Common.hpp"

class InputManager {
public:
	InputManager();
	~InputManager();
	void ProcessInputEvent(const SDL_Event& event);
	void EndOfFrame();
	int GetKeyState(std::string keyName);
private:
	Uint8 keyStates[SDL_SCANCODE_COUNT] = { 0 };
};