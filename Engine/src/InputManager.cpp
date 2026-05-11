#include "InputManager.hpp"
#include <SDL3/SDL.h>

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::ProcessInputEvent(const SDL_Event& event){

	if (event.key.type != SDL_EVENT_KEY_DOWN && event.key.type != SDL_EVENT_KEY_UP) return;

	SDL_Scancode scancode = event.key.scancode;

	if (event.key.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.repeat) {
            keyStates[scancode] = 1;
        } else {
            keyStates[scancode] = 2;
        }
    } else {
        keyStates[scancode] = 0;
    }
}

void InputManager::EndOfFrame(){
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        if (keyStates[i] == 2) {
            keyStates[i] = 1;
        }
	}
}

int InputManager::GetKeyState(std::string keyName) {
    return keyStates[SDL_GetScancodeFromName((keyName).c_str())];
}
