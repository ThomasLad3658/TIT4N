#include "LuaManager.hpp"
#include "Game.hpp"

LuaManager::LuaManager() {
	L = luaL_newstate();
	luaL_openlibs(L);
}

LuaManager::~LuaManager() {
	lua_close(L);
}

void LuaManager::registerFunctions() {
	lua_register(L, "SetWindowTitle", lua_SetWindowTitle);
	lua_register(L, "SetWindowSize", lua_SetWindowSize);
}

bool LuaManager::DoFile(const char* path) {
	if (luaL_dofile(L, path) != LUA_OK) {
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return false;
	}
	else return true;
}

int LuaManager::lua_SetWindowTitle(lua_State* L) {
	const char* title = lua_tostring(L, 1);
	ServiceLocater::getGame()->SetWindowTitle(title);
	return 0;
}

int LuaManager::lua_SetWindowSize(lua_State* L) {
	int width = lua_tointeger(L, 1);
	int height = lua_tointeger(L, 2);
	ServiceLocater::getGame()->SetWindowSize(width, height);
	return 0;
}
