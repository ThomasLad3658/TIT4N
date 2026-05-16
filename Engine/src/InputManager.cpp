#include "InputManager.hpp"
#include <SDL3/SDL.h>

InputManager::InputManager() {
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        keyStates[i] = 0;
    }

    mouseX = 0.0f;
    mouseY = 0.0f;

    mouseWheelX = 0.0f;
    mouseWheelY = 0.0f;

    for (int i = 0; i < 5; i++) {
        mouseButtonStates[i] = 0;
    }
}

InputManager::~InputManager()
{
}

void InputManager::ProcessInputEvent(const SDL_Event& event){

	switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION:
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            mouseWheelX = event.wheel.x;
            mouseWheelY = event.wheel.y;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if(mouseButtonStates[event.button.button - 1] == 2)
                mouseButtonStates[event.button.button - 1] = 1;
            else 
                mouseButtonStates[event.button.button - 1] = 2;
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            mouseButtonStates[event.button.button - 1] = 0;
            break;
		case SDL_EVENT_KEY_DOWN:
            if (event.key.repeat) {
                keyStates[event.key.scancode] = 1;
            }
            else {
                keyStates[event.key.scancode] = 2;
            }
            break;
        case SDL_EVENT_KEY_UP:
            keyStates[event.key.scancode] = 0;
			break;
    }
}

void InputManager::EndOfFrame(){
    for (int i = 0; i < SDL_SCANCODE_COUNT; i++) {
        if (keyStates[i] == 2) {
            keyStates[i] = 1;
        }
	}
    for (int i = 0; i < 5; i++) {
        if (mouseButtonStates[i] == 2) {
            mouseButtonStates[i] = 1;
        }
    }
}

int InputManager::GetKeyState(std::string keyName) {
    return keyStates[SDL_GetScancodeFromName((keyName).c_str())];
}

float InputManager::GetMouseState(std::string mouseInput) {
	if (mouseInput == "x") return mouseX;
    else if(mouseInput == "y") return mouseY;
    else if(mouseInput == "wheelX") return mouseWheelX;
    else if(mouseInput == "wheelY") return mouseWheelY;
    else if(mouseInput == "leftButton") return mouseButtonStates[0];
    else if(mouseInput == "rightButton") return mouseButtonStates[1];
    else if(mouseInput == "middleButton") return mouseButtonStates[2];
    else if(mouseInput == "button4") return mouseButtonStates[3];
    else if(mouseInput == "button5") return mouseButtonStates[4];
    else {
        std::cerr << "Invalid mouse input name : " << mouseInput << std::endl;
        throw std::runtime_error("Invalid mouse input name");
    }
}
