#include "LuaManager.hpp"
#include "Game.hpp"

template <typename T, typename S>
T lua_get(lua_State* L, int index) {
	//
}

template<>
int lua_get<int>(lua_State* L, int index) {
	return static_cast<int>(lua_tointeger(L, index));
}

template<>
double lua_get<double>(lua_State* L, int index) {
	return lua_tonumber(L, index);
}

template<>
float lua_get<float>(lua_State* L, int index) {
	return static_cast<float>(lua_tonumber(L, index));
}

template<>
bool lua_get<bool>(lua_State* L, int index) {
	return lua_toboolean(L, index);
}

template<>
const char* lua_get<const char*>(lua_State* L, int index) {
	return lua_tostring(L, index);
}

template<>
std::string lua_get<std::string>(lua_State* L, int index) {
	return lua_tostring(L, index);
}

template <typename T>
void lua_push(lua_State* L, T value) {
	//
}

template <>
void lua_push<int>(lua_State* L, int value) {
	lua_pushinteger(L, value);
}

template <>
void lua_push<float>(lua_State* L, float value) {
	lua_pushnumber(L, static_cast<lua_Number>(value));
}

template <>
void lua_push<double>(lua_State* L, double value) {
	lua_pushnumber(L, value);
}

template <>
void lua_push<bool>(lua_State* L, bool value) {
	lua_pushboolean(L, value);
}

template <>
void lua_push<const char*>(lua_State* L, const char* value) {
	lua_pushstring(L, value);
}

LuaManager::LuaManager() {
	L = luaL_newstate();
	luaL_openlibs(L);
}

LuaManager::~LuaManager() {
	lua_close(L);
}

bool LuaManager::DoFile(const char* path) {
	if (luaL_dofile(L, path) != LUA_OK) {
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return false;
	}
	else return true;
}