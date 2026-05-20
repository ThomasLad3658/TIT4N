#include "Game.hpp"
#include "SceneManager.hpp"
#include "ServiceLocator.hpp"
#include "RenderSystem.hpp"
#include "LuaManager.hpp"
#include "Common.hpp"

SceneManager::SceneManager(std::vector<std::unique_ptr<Entity>>* entities) : entities(entities) {
	queuedScene = "";
}

void SceneManager::Update() {
	if (queuedScene != "") {
		LoadScene(queuedScene);
		queuedScene = "";
	}
}

void SceneManager::QueueScene(std::string name) {
	queuedScene = name;
}

void SceneManager::LoadScene(std::string name) {
	std::string path = (getBasePath() + "Game/" + name + ".lua");
	std::cout << "Loading : " << path << std::endl;

	ClearScene();

	LuaManager* luaManager = ServiceLocator::getLuaManager();
	std::string tablePath = name.substr(name.find_last_of('/') + 1);
	luaManager->DoFile(path.c_str());
	int size = luaManager->GetFieldSize((tablePath + ".entities"));
	for (int i = 1; i <= size; i++) {

		std::string entityPath = (tablePath + ".entities.entity" + std::to_string(i));

		std::unique_ptr<Entity> entity = ServiceLocator::getGame()->CreateEntity(entityPath);
		entity->Init(ServiceLocator::getRenderSystem()->getRenderer());
		ServiceLocator::getGame()->registerEntity(std::move(entity));

	}
}

void SceneManager::ClearScene() {
	entities->clear();
}
