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
	template <typename R, typename O, typename... Args>
	void RegisterFunctionToLuaField(O* obj, R(O::* func)(Args...), const char* fieldPath, const char* name);
	bool DoFile(const char* path);
	template <typename T, typename... Args>
	T callFunction(const char* name, bool requiresSelf, Args... args);
	template <typename T>
	T GetVariable(const char* name);
	int ReferenceNewObjWithPath(const char* blueprintName, const char* overridesPath);
	void DereferenceObj(int ref);
private:
	lua_State* L = nullptr;
	bool GetFields(std::string fieldsPath);
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
}

template <typename R, typename O, typename... Args>
void LuaManager::RegisterFunction(O* obj, R(O::*func)(Args...), const char* name) {
	RegisterHelper(obj, func, std::make_index_sequence<sizeof...(Args)>{}, name);
	lua_setglobal(L, name);
}

template <typename R, typename O, typename... Args>
void LuaManager::RegisterFunctionToLuaField(O* obj, R(O::* func)(Args...), const char * fieldPath, const char* name) {
	GetFields(fieldPath);
	RegisterHelper(obj, func, std::make_index_sequence<sizeof...(Args)>{}, name);
	lua_setfield(L, -2, name);
}

template <typename T, typename... Args>
T LuaManager::callFunction(const char* name, bool requiresSelf, Args... args) {

	if (requiresSelf) {
		std::string nameStr = name;
		if (nameStr.find('.') == std::string::npos) {
			std::cerr << "Function name must be in the format 'table.function' if requiresSelf is true : " << name << std::endl;
			throw std::runtime_error("Invalid function name format");
		}
		else {
			if (GetFields(name) == false) {
				std::cerr << "Couldn't find path for function : " << name << std::endl;
				throw std::runtime_error("Couldn't find path for function");
			}
			std::stringstream ss(nameStr);
			std::vector<std::string> fields;
			std::string fieldPart;
			while (std::getline(ss, fieldPart, '.')) {
				fields.push_back(fieldPart);
			}
			std::string funcTablePath;
			funcTablePath += fields[0];
			for (int i = 1; i < (int)fields.size() - 1; i++) {
				funcTablePath += ".";
				funcTablePath += fields[i];
			}
			if (GetFields(funcTablePath) == false) {
				std::cerr << "Couldn't find table for function : " << name << std::endl;
				throw std::runtime_error("Couldn't find table for function");
			}
		}
	}
	else {
		GetFields(name);
	}

	(lua_push(L, args), ...);

	int numArgs = sizeof...(args);
	if (requiresSelf) {
		numArgs++;
	}
	bool result = false;
	if constexpr (!std::is_void_v<T>) result = true;
	
	if (lua_pcall(L, numArgs, result, 0) != LUA_OK) {
		std::cerr << "Couldn't call Lua function : " << name << " : " << lua_tostring(L, -1) << std::endl;
		throw std::runtime_error("Couldn't call Lua function");
	}
	if constexpr (!std::is_void_v<T>) {
		T value = lua_get<T>(L, -1);
		return value;
	}
	else {
		return T{};
	}
}

template <typename T>
T LuaManager::GetVariable(const char* name) {
	GetFields(name);
	T answer = lua_get<T>(L, -1);
	return answer;
}