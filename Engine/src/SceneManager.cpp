#include "SceneManager.hpp"
#include "ServiceLocator.hpp"
#include "LuaManager.hpp"
#include "Common.hpp"

SceneManager::SceneManager(std::vector<Entity*>* entities) : entities(entities) {
	//
}

void SceneManager::LoadLevel(std::string name) {
	std::string levelPath = (getBasePath() + "Game\\levels\\" + name + ".lua");
	std::cout << "Loading level : " << levelPath << std::endl;

	// *Note* add something to clean everything before loading

	LuaManager* luaManager = ServiceLocator::getLuaManager();
	luaManager->DoFile(levelPath.c_str());


}
