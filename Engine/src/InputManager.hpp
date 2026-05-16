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
	float GetMouseState(std::string mouseInput);
private:
	Uint8 keyStates[SDL_SCANCODE_COUNT];

	float mouseX;
	float mouseY;

	float mouseWheelX;
	float mouseWheelY;

	Uint8 mouseButtonStates[5];
};