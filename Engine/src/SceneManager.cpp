#include "SceneManager.hpp"

SceneManager::SceneManager(std::vector<Entity*>* entities) : entities(entities) {
	//
}

void SceneManager::LoadLevel(std::string name) {
	std::cout << "Loading level : " << name << std::endl;
}