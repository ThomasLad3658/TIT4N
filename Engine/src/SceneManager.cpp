#include "Game.hpp"
#include "SceneManager.hpp"
#include "ServiceLocator.hpp"
#include "RenderSystem.hpp"
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
	for (int i = 1; i <= luaManager->GetVariable<int>("level.entityCount"); i++) {
		Entity* entity = new Entity(
			luaManager->GetVariable<std::string>(("level.entities.entity" + std::to_string(i) + ".path").c_str()),
			SDL_FRect{
				luaManager->GetVariable<float>(("level.entities.entity" + std::to_string(i) + ".srcrect.x").c_str()),
				luaManager->GetVariable<float>(("level.entities.entity" + std::to_string(i) + ".srcrect.y").c_str()),
				luaManager->GetVariable<float>(("level.entities.entity" + std::to_string(i) + ".srcrect.w").c_str()),
				luaManager->GetVariable<float>(("level.entities.entity" + std::to_string(i) + ".srcrect.h").c_str())
			},
			SDL_FRect{
				luaManager->GetVariable<float>(("level.entities.entity" + std::to_string(i) + ".dstrect.x").c_str()),
				luaManager->GetVariable<float>(("level.entities.entity" + std::to_string(i) + ".dstrect.y").c_str()),
				luaManager->GetVariable<float>(("level.entities.entity" + std::to_string(i) + ".dstrect.w").c_str()),
				luaManager->GetVariable<float>(("level.entities.entity" + std::to_string(i) + ".dstrect.h").c_str())
			}
		);
		entity->Init(ServiceLocator::getRenderSystem()->getRenderer());
		ServiceLocator::getGame()->registerEntity(entity);
	}
}
