#pragma once
#include <iostream>
#include <SDL3/SDL.h>

std::string getBasePath();

//   0 = not pressed
//   1 = pressed
//   2 = just pressed (pressed this frame but not last frame)
struct Keys {
    // Letters A-Z
    Uint8 K_A = 0; Uint8 K_B = 0; Uint8 K_C = 0; Uint8 K_D = 0;
    Uint8 K_E = 0; Uint8 K_F = 0; Uint8 K_G = 0; Uint8 K_H = 0;
    Uint8 K_I = 0; Uint8 K_J = 0; Uint8 K_K = 0; Uint8 K_L = 0;
    Uint8 K_M = 0; Uint8 K_N = 0; Uint8 K_O = 0; Uint8 K_P = 0;
    Uint8 K_Q = 0; Uint8 K_R = 0; Uint8 K_S = 0; Uint8 K_T = 0;
    Uint8 K_U = 0; Uint8 K_V = 0; Uint8 K_W = 0; Uint8 K_X = 0;
    Uint8 K_Y = 0; Uint8 K_Z = 0;

    // Numbers 0-9
    Uint8 K_0 = 0; Uint8 K_1 = 0; Uint8 K_2 = 0; Uint8 K_3 = 0;
    Uint8 K_4 = 0; Uint8 K_5 = 0; Uint8 K_6 = 0; Uint8 K_7 = 0;
    Uint8 K_8 = 0; Uint8 K_9 = 0;

    // Function keys
    Uint8 K_F1 = 0;  Uint8 K_F2 = 0;  Uint8 K_F3 = 0;  Uint8 K_F4 = 0;
    Uint8 K_F5 = 0;  Uint8 K_F6 = 0;  Uint8 K_F7 = 0;  Uint8 K_F8 = 0;
    Uint8 K_F9 = 0;  Uint8 K_F10 = 0; Uint8 K_F11 = 0; Uint8 K_F12 = 0;

    // Keypad keys (NumLock off/on both included as standard keys)
    Uint8 K_KP_0 = 0; Uint8 K_KP_1 = 0; Uint8 K_KP_2 = 0; Uint8 K_KP_3 = 0;
    Uint8 K_KP_4 = 0; Uint8 K_KP_5 = 0; Uint8 K_KP_6 = 0; Uint8 K_KP_7 = 0;
    Uint8 K_KP_8 = 0; Uint8 K_KP_9 = 0;
    Uint8 K_KP_PERIOD = 0;    // .
    Uint8 K_KP_DIVIDE = 0;    // /
    Uint8 K_KP_MULTIPLY = 0;  // *
    Uint8 K_KP_MINUS = 0;     // -
    Uint8 K_KP_PLUS = 0;      // +
    Uint8 K_KP_ENTER = 0;
    Uint8 K_KP_EQUALS = 0;

    // Arrow keys
    Uint8 K_UP = 0; Uint8 K_DOWN = 0; Uint8 K_LEFT = 0; Uint8 K_RIGHT = 0;

    // Modifier keys (left and right variants)
    Uint8 K_LSHIFT = 0; Uint8 K_RSHIFT = 0;
    Uint8 K_LCTRL = 0; Uint8 K_RCTRL = 0;
    Uint8 K_LALT = 0; Uint8 K_RALT = 0;
    Uint8 K_LGUI = 0; Uint8 K_RGUI = 0;  // Windows / Command key

    // Editing / navigation keys
    Uint8 K_BACKSPACE = 0; Uint8 K_TAB = 0;
    Uint8 K_RETURN = 0;     // same as K_ENTER
    Uint8 K_ESCAPE = 0;
    Uint8 K_SPACE = 0;
    Uint8 K_DELETE = 0;     Uint8 K_INSERT = 0;
    Uint8 K_HOME = 0;       Uint8 K_END = 0;
    Uint8 K_PAGEUP = 0;     Uint8 K_PAGEDOWN = 0;

    // Lock keys (often toggles, but state can be tracked)
    Uint8 K_CAPSLOCK = 0;   Uint8 K_NUMLOCK = 0;   Uint8 K_SCROLLLOCK = 0;

    // Miscellaneous keys
    Uint8 K_PRINTSCREEN = 0; Uint8 K_PAUSE = 0;
    Uint8 K_MENU = 0;        // right-click / context menu key

    // Punctuation and symbols (US keyboard layout)
    Uint8 K_GRAVE = 0;       // ` ~
    Uint8 K_MINUS = 0;       // - _
    Uint8 K_EQUALS = 0;      // = +
    Uint8 K_LEFTBRACKET = 0; // [ {
    Uint8 K_RIGHTBRACKET = 0;// ] }
    Uint8 K_BACKSLASH = 0;   // \ |
    Uint8 K_SEMICOLON = 0;   // ; :
    Uint8 K_APOSTROPHE = 0;  // ' "
    Uint8 K_COMMA = 0;       // , <
    Uint8 K_PERIOD = 0;      // . >
    Uint8 K_SLASH = 0;       // / ?

    // Constructor: zero-initializes all members (including any future additions)
    Keys() {
        // Use memset to set all bytes to 0. This works because the struct is trivially copyable.
        SDL_memset(this, 0, sizeof(*this));
    }
};