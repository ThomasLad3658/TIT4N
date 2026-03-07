#pragma once
#include <iostream>
#include "ServiceLocator.hpp"
extern "C" {
#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>
}

class LuaManager{
public:
	LuaManager();
	~LuaManager();
	void RegisterFunctions();
	bool DoFile(const char* path);
	static int lua_SetWindowTitle(lua_State* L);
	static int lua_SetWindowSize(lua_State* L);
private:
	lua_State* L = nullptr;
};

