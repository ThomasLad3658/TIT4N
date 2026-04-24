#include "LuaManager.hpp"
#include "Game.hpp"
#include "Entity.hpp"

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

// tag.c_str(), (name + ".entities.entity" + std::to_string(i)).c_str()

int LuaManager::ReferenceNewObjWithPath(const char* blueprintName, const char* overridesPath) {

	lua_getglobal(L, blueprintName);
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		throw std::runtime_error(std::string("Blueprint not found: ") + blueprintName);
	}

	lua_getfield(L, -1, "new");
	if (!lua_isfunction(L, -1)) {
		lua_pop(L, 2);
		throw std::runtime_error(std::string("Blueprint has no 'new' method: ") + blueprintName);
	}

	lua_pushvalue(L, -2);
	
	std::string nameStr = overridesPath;
	if (nameStr.find('.') == std::string::npos) {
		lua_getglobal(L, overridesPath);
		lua_pushvalue(L, -3);
		lua_pushvalue(L, -2);
	}
	else {
		std::stringstream ss(nameStr);
		std::vector<std::string> fields;
		std::string fieldPart;

		while (std::getline(ss, fieldPart, '.')) {
			fields.push_back(fieldPart);
		}

		lua_getglobal(L, fields[0].c_str());

		for (int i = 1; i < (int)fields.size(); i++) {
			lua_getfield(L, -1, fields[i].c_str());
		}

		for (int i = 0; i < (int)fields.size() - 1; i++) {
			lua_remove(L, -2);
		}
	}

	if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
		std::cerr << "Couldn't create metatable for Lua table : " << blueprintName << " from path : " << overridesPath << " : " << lua_tostring(L, -1) << std::endl;
		throw std::runtime_error("Couldn't create metatable");
	}
	lua_remove(L, -2);

	return luaL_ref(L, LUA_REGISTRYINDEX);

}

void LuaManager::DereferenceObj(int ref) {
	luaL_unref(L, LUA_REGISTRYINDEX, ref);
}