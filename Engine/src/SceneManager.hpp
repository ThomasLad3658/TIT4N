#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "Entity.hpp"

class SceneManager{
public :
	SceneManager(std::vector<std::unique_ptr<Entity>>* entities);
	void LoadLevel(std::string name);
private:
	std::vector<std::unique_ptr<Entity>>* entities;
};
