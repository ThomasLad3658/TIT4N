#include "ServiceLocater.hpp"
#include "Game.hpp"
#include "SceneManager.hpp"
#include "RenderSystem.hpp"
#include "PhysicsSystem.hpp"
#include "LuaManager.hpp"

Game* ServiceLocater::game = nullptr;
SceneManager* ServiceLocater::sceneManager = nullptr;
RenderSystem* ServiceLocater::renderSystem = nullptr;
PhysicsSystem* ServiceLocater::physicsSystem = nullptr;
LuaManager* ServiceLocater::luaManager = nullptr;

void ServiceLocater::registerGame(Game* g) {
	game = g;
}

void ServiceLocater::registerSceneManager(SceneManager* sm) {
	sceneManager = sm;
}

void ServiceLocater::registerRenderSystem(RenderSystem* rs) {
	renderSystem = rs;
}

void ServiceLocater::registerPhysicsSystem(PhysicsSystem* ps) {
	physicsSystem = ps;
}

void ServiceLocater::registerLuaManager(LuaManager* lm) {
	luaManager = lm;
}

Game* ServiceLocater::getGame() {
	return game;
}

SceneManager* ServiceLocater::getSceneManager() {
	return sceneManager;
}

RenderSystem* ServiceLocater::getRenderSystem() {
	return renderSystem;
}

PhysicsSystem* ServiceLocater::getPhysicsSystem() {
	return physicsSystem;
}

LuaManager* ServiceLocater::getLuaManager() {
	return luaManager;
}
