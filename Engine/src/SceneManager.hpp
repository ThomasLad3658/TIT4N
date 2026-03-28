#pragma once
#include <iostream>
#include <vector>
#include "Entity.hpp"

class SceneManager{
public :
	SceneManager(std::vector<Entity*>* entities);
	void LoadLevel(std::string name);
private:
	std::vector<Entity*>* entities;
};
