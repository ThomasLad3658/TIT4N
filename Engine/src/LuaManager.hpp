#pragma once
#include <iostream>
#include <type_traits>
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
	template <typename R, typename... Args>
	void RegisterFunction(R(*func)(Args...), const char* name);
	bool DoFile(const char* path);
private:
	lua_State* L = nullptr;
	template <typename R, typename... Args, size_t... I>
	void RegisterHelper(R(*func)(Args...), std::index_sequence<I...>, const char *name);
};

template <typename T>
T lua_get(lua_State* L, int index);

template<>
int lua_get<int>(lua_State* L, int index);

template<>
double lua_get<double>(lua_State* L, int index);

template<>
float lua_get<float>(lua_State* L, int index);

template<>
bool lua_get<bool>(lua_State* L, int index);

template<>
const char* lua_get<const char*>(lua_State* L, int index);

template <typename T>
void lua_push(lua_State* L, T value);

template <>
void lua_push<int>(lua_State* L, int value);

template <>
void lua_push<float>(lua_State* L, float value);

template <>
void lua_push<double>(lua_State* L, double value);

template <>
void lua_push<bool>(lua_State* L, bool value);

template <>
void lua_push<const char*>(lua_State* L, const char* value);

template <typename R, typename... Args, size_t... I>
void LuaManager::RegisterHelper(R(*func)(Args...), std::index_sequence<I...>, const char* name) {
	lua_pushlightuserdata(L, reinterpret_cast<void*>(func));
	lua_pushcclosure(L, [](lua_State* L) -> int {
		R(*func)(Args...) = reinterpret_cast<R(*)(Args...)>(lua_touserdata(L, lua_upvalueindex(1)));
		if constexpr (std::is_void_v<R>) {
			func(lua_get<Args>(L, I + 1)...);
			return 0;
		}
		else {
			R rvalue = func(lua_get<Args>(L, I + 1)...);
			lua_push(L, rvalue);
			return 1;
		}
		}, 1);
	lua_setglobal(L, name);
}

template <typename R, typename... Args>
void LuaManager::RegisterFunction(R(*func)(Args...), const char* name) {
	RegisterHelper(func, std::make_index_sequence<sizeof...(Args)>{}, name);
}