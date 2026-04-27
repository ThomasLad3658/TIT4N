#include "Game.hpp"
#include "SceneManager.hpp"
#include "ServiceLocator.hpp"
#include "RenderSystem.hpp"
#include "LuaManager.hpp"
#include "Common.hpp"

SceneManager::SceneManager(std::vector<std::unique_ptr<Entity>>* entities) : entities(entities) {
	//
}

void SceneManager::LoadLevel(std::string name) {
	std::string levelPath = (getBasePath() + "Game/levels/" + name + ".lua");
	std::cout << "Loading level : " << levelPath << std::endl;

	// *Note* add something to clean everything before loading

	LuaManager* luaManager = ServiceLocator::getLuaManager();
	luaManager->DoFile(levelPath.c_str());
	for (int i = 1; i <= luaManager->GetVariable<int>((name + ".entityCount").c_str()); i++) {

		std::string entityPath = (name + ".entities.entity" + std::to_string(i));

		std::unique_ptr<Entity> entity = ServiceLocator::getGame()->CreateEntity(entityPath);
		entity->Init(ServiceLocator::getRenderSystem()->getRenderer());
		ServiceLocator::getGame()->registerEntity(std::move(entity));

	}
}
