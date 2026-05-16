#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "Entity.hpp"

class SceneManager{
public :
	SceneManager(std::vector<std::unique_ptr<Entity>>* entities);
	void LoadScene(std::string name);
	void ClearScene();
private:
	std::vector<std::unique_ptr<Entity>>* entities;
};
