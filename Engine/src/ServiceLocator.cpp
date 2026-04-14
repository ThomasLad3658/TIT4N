#include "ServiceLocator.hpp"
#include "Game.hpp"
#include "SceneManager.hpp"
#include "RenderSystem.hpp"
#include "PhysicsSystem.hpp"
#include "LuaManager.hpp"
#include "SaveSystem.hpp"

Game* ServiceLocator::game = nullptr;
SceneManager* ServiceLocator::sceneManager = nullptr;
RenderSystem* ServiceLocator::renderSystem = nullptr;
PhysicsSystem* ServiceLocator::physicsSystem = nullptr;
LuaManager* ServiceLocator::luaManager = nullptr;
SaveSystem* ServiceLocator::saveSystem = nullptr;

void ServiceLocator::registerGame(Game* g) {
	game = g;
}

void ServiceLocator::registerSceneManager(SceneManager* sm) {
	sceneManager = sm;
}

void ServiceLocator::registerRenderSystem(RenderSystem* rs) {
	renderSystem = rs;
}

void ServiceLocator::registerPhysicsSystem(PhysicsSystem* ps) {
	physicsSystem = ps;
}

void ServiceLocator::registerLuaManager(LuaManager* lm) {
	luaManager = lm;
}

void ServiceLocator::registerSaveSystem(SaveSystem* ss) {
	saveSystem = ss;
}

Game* ServiceLocator::getGame() {
	return game;
}

SceneManager* ServiceLocator::getSceneManager() {
	return sceneManager;
}

RenderSystem* ServiceLocator::getRenderSystem() {
	return renderSystem;
}

PhysicsSystem* ServiceLocator::getPhysicsSystem() {
	return physicsSystem;
}

LuaManager* ServiceLocator::getLuaManager() {
	return luaManager;
}

SaveSystem* ServiceLocator::getSaveSystem() {
	return saveSystem;
}
