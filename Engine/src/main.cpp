#include "Game.hpp"

extern "C" {
#include <Lua/lua.h>
#include <Lua/lauxlib.h>
#include <Lua/lualib.h>
}

int main(int argc, char* argv[]) {
	Game game;
 	game.Run();
	return 0;
}