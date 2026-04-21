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
	for (int i = 1; i <= luaManager->GetVariable<int>((name + ".entityCount").c_str()); i++) {
		std::string tag = luaManager->GetVariable<std::string>((name + ".entities.entity" + std::to_string(i) + ".tag").c_str());
		luaManager->DoFile((getBasePath() + "Game\\scripts\\" + tag + ".lua").c_str());
		float dstScale = luaManager->GetVariable<float>((tag + ".dstScale").c_str());
		float w = luaManager->GetVariable<float>((tag + ".srcrect.w").c_str());
		float h = luaManager->GetVariable<float>((tag + ".srcrect.h").c_str());
		Entity* entity = new Entity(
			tag,
			luaManager->GetVariable<std::string>((tag + ".path").c_str()),
			SDL_FRect{
				luaManager->GetVariable<float>((tag + ".srcrect.x").c_str()),
				luaManager->GetVariable<float>((tag + ".srcrect.y").c_str()),
				w,
				h
			},
			SDL_FRect{
				luaManager->GetVariable<float>((name + ".entities.entity" + std::to_string(i) + ".x").c_str()),
				luaManager->GetVariable<float>((name + ".entities.entity" + std::to_string(i) + ".y").c_str()),
				dstScale * w,
				dstScale * h
			}
		);
		entity->Init(ServiceLocator::getRenderSystem()->getRenderer());
		ServiceLocator::getGame()->registerEntity(entity);
	}
}
