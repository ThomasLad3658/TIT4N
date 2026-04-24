#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <type_traits>
#include "ServiceLocator.hpp"
#include "Common.hpp"
extern "C" {
#include <Lua/lua.h>
#include <Lua/lualib.h>
#include <Lua/lauxlib.h>
}

class Entity;

class LuaManager{
public:
	LuaManager();
	~LuaManager();
	template <typename R, typename O, typename... Args>
	void RegisterFunction(O* obj, R(O::*func)(Args...), const char* name);
	bool DoFile(const char* path);
	template <typename T, typename... Args>
	T callFunction(const char* name, Args... args);
	template <typename T>
	T GetVariable(const char* name);
	int ReferenceNewObjWithPath(const char* blueprintName, const char* overridesPath);
	void DereferenceObj(int ref);
private:
	lua_State* L = nullptr;
	template <typename R, typename O, typename... Args, size_t... I>
	void RegisterHelper(O* obj, R(O::*func)(Args...), std::index_sequence<I...>, const char *name);
	std::vector<std::shared_ptr<void>>allocatedFuncPtrs;
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

template<>
std::string lua_get<std::string>(lua_State* L, int index);

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

template <typename R, typename O, typename... Args, size_t... I>
void LuaManager::RegisterHelper(O* obj, R(O:: * func)(Args...), std::index_sequence<I...>, const char* name) {
	std::shared_ptr funcPtr = std::make_shared<R(O::*)(Args...)>(func);
	allocatedFuncPtrs.push_back(funcPtr);
	lua_pushlightuserdata(L, reinterpret_cast<void*>(obj));
	lua_pushlightuserdata(L, reinterpret_cast<void*>(funcPtr.get()));
	lua_pushcclosure(L, [](lua_State* L) -> int {
		auto obj = reinterpret_cast<O*>(lua_touserdata(L, lua_upvalueindex(1)));
		auto func = reinterpret_cast<R(O::**)(Args...)>(lua_touserdata(L, lua_upvalueindex(2)));
		if constexpr (std::is_void_v<R>) {
			(obj->*(*func))(lua_get<Args>(L, I + 1)...);
			return 0;
		}
		else {
			R rvalue = (obj->*(*func))(lua_get<Args>(L, I + 1)...);
			lua_push(L, rvalue);
			return 1;
		}
	}, 2);
	lua_setglobal(L, name);
}

template <typename R, typename O, typename... Args>
void LuaManager::RegisterFunction(O* obj, R(O::*func)(Args...), const char* name) {
	RegisterHelper(obj, func, std::make_index_sequence<sizeof...(Args)>{}, name);
}

template <typename T, typename... Args>
T LuaManager::callFunction(const char* name, Args... args) {
	bool requiresSelf = false;

	std::string nameStr = name;
	if (nameStr.find('.') == std::string::npos) {
		lua_getglobal(L, name);
		(lua_push(L, args), ...);
		if constexpr (!std::is_void_v<T>) {
			if (lua_pcall(L, sizeof...(args), 1, 0) != LUA_OK) {
				std::cerr << "Couldn't call Lua function : " << name << " : " << lua_tostring(L, -1) << std::endl;
				throw std::runtime_error("Couldn't call Lua function");
			}
			T value = lua_get<T>(L, -1);
			lua_pop(L, 1);
			return value;
		}
		else {
			if (lua_pcall(L, sizeof...(args), 0, 0) != LUA_OK) {
				std::cerr << "Couldn't call Lua function : " << name << " : " << lua_tostring(L, -1) << std::endl;
				throw std::runtime_error("Couldn't call Lua function");
			}
			return T{};
		}
	}

	std::stringstream ss(nameStr);
	std::vector<std::string> fields;
	std::string fieldPart;

	while (std::getline(ss, fieldPart, '.')) {
		fields.push_back(fieldPart);
	}

	if (fields[0].find("/") != std::string::npos) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, std::stoi(fields[0].erase(0, 1)));
		requiresSelf = true;
	}
	else {
		lua_getglobal(L, fields[0].c_str());
	}
	for (int i = 1; i < (int)fields.size(); i++) {
		if (fields[i].find("/") != std::string::npos) {
			lua_rawgeti(L, LUA_REGISTRYINDEX, std::stoi(fields[i].erase(0, 1)));
			requiresSelf = true;
		}
		else {
			lua_getfield(L, -1, fields[i].c_str());
		}
	}

	if (requiresSelf) lua_pushvalue(L, -2);
	(lua_push(L, args), ...);

	if constexpr (!std::is_void_v<T>) {
		int res;
		if (requiresSelf) {
			res = lua_pcall(L, sizeof...(args) + 1, 1, 0);
		}
		else {
			res = lua_pcall(L, sizeof...(args), 1, 0);
		}
		if (res != LUA_OK) {
			std::cerr << "Couldn't call Lua function : " << name << " : " << lua_tostring(L, -1) << std::endl;
			throw std::runtime_error("Couldn't call Lua function");
		}
		T value = lua_get<T>(L, -1);
		lua_pop(L, (int)fields.size());
		return value;
	}
	else {
		int res;
		if (requiresSelf) {
			res = lua_pcall(L, sizeof...(args) + 1, 1, 0);
		}
		else {
			res = lua_pcall(L, sizeof...(args), 1, 0);
		}
		if (res != LUA_OK) {
			std::cerr << "Couldn't call Lua function : " << name << " : " << lua_tostring(L, -1) << std::endl;
			throw std::runtime_error("Couldn't call Lua function");
		}
		lua_pop(L, (int)fields.size() - 1);
		return T{};
	}
}

/*
template <typename T, typename... Args>
T LuaManager::callFunction(const char* name, Args... args) {

	bool requiresSelf = false;

	std::string nameStr = name;
	if (nameStr.find('.') == std::string::npos) {
		lua_getglobal(L, name);
		(lua_push(L, args), ...);
		if constexpr (!std::is_void_v<T>) {
			if (lua_pcall(L, sizeof...(args), 1, 0) != LUA_OK) {
				std::cerr << "Couldn't call Lua function : " << name << " : " << lua_tostring(L, -1) << std::endl;
				throw std::runtime_error("Couldn't call Lua function");
			}
			T value = lua_get<T>(L, -1);
			lua_pop(L, 1);
			return value;
		}
		else {
			if (lua_pcall(L, sizeof...(args), 0, 0) != LUA_OK) {
				std::cerr << "Couldn't call Lua function : " << name << " : " << lua_tostring(L, -1) << std::endl;
				throw std::runtime_error("Couldn't call Lua function");
			}
			return T{};
		}
	}

	std::stringstream ss(nameStr);
	std::vector<std::string> fields;
	std::string fieldPart;

	while (std::getline(ss, fieldPart, '.')) {
		fields.push_back(fieldPart);
	}

	if (fields[0].find("/") != std::string::npos) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, std::stoi(fields[0].erase(0, 1)));
		requiresSelf = true;
	}
	else {
		lua_getglobal(L, fields[0].c_str());
	}

	for (int i = 1; i < (int)fields.size(); i++) {
		if (fields[i].find("/") != std::string::npos) {
			lua_rawgeti(L, LUA_REGISTRYINDEX, std::stoi(fields[i].erase(0, 1)));
			requiresSelf = true;
		}
		else {
			lua_getfield(L, -1, fields[i].c_str());
		}
	}
	if (requiresSelf) lua_pushvalue(L, -2);

	(lua_push(L, args), ...);
	if constexpr (!std::is_void_v<T>) {
		int res;
		if (requiresSelf) {
			res = lua_pcall(L, sizeof...(args) + 1, 1, 0);
		}
		else {
			res = lua_pcall(L, sizeof...(args), 1, 0);
		}
		if (res != LUA_OK) {
			std::cerr << "Couldn't call Lua function : " << name << " : " << lua_tostring(L, -1) << std::endl;
			throw std::runtime_error("Couldn't call Lua function");
		}
		T value = lua_get<T>(L, -1);
		lua_pop(L, (int)fields.size());
		if (requiresSelf) lua_pop(L, 1);
		return value;
	}
	else {
		if (lua_pcall(L, sizeof...(args), 0, 0) != LUA_OK) {
			std::cerr << "Couldn't call Lua function : " << name << " : " << lua_tostring(L, -1) << std::endl;
			throw std::runtime_error("Couldn't call Lua function");
		}
		lua_pop(L, (int)fields.size() - 1);
		if (requiresSelf) lua_pop(L, 1);
		return T{};
	}

}
*/

template <typename T>
T LuaManager::GetVariable(const char* name) {
	bool requiresSelf = false;

	std::string nameStr = name;
	if (nameStr.find('.') == std::string::npos) {
		lua_getglobal(L, name);
		T value = lua_get<T>(L, -1);
		lua_pop(L, 1);
		return value;
	}

	std::stringstream ss(nameStr);
	std::vector<std::string> fields;
	std::string fieldPart;

	while (std::getline(ss, fieldPart, '.')) {
		fields.push_back(fieldPart);
	}

	if (fields[0].find("/") != std::string::npos) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, std::stoi(fields[0].erase(0, 1)));
		requiresSelf = true;
	}
	else {
		lua_getglobal(L, fields[0].c_str());
	}

	for (int i = 1; i < (int)fields.size(); i++) {
		if (fields[i].find("/") != std::string::npos) {
			lua_rawgeti(L, LUA_REGISTRYINDEX, std::stoi(fields[i].erase(0, 1)));
			requiresSelf = true;
		}
		else {
			lua_getfield(L, -1, fields[i].c_str());
		}
	}

	T answer = lua_get<T>(L, -1);
	
	lua_pop(L, (int)fields.size());

	return answer;
}