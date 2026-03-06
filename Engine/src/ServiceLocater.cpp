#include "ServiceLocater.hpp"

SceneManager* ServiceLocater::sceneManager = nullptr;
RenderSystem* ServiceLocater::renderSystem = nullptr;
PhysicsSystem* ServiceLocater::physicsSystem = nullptr;
LuaManager* ServiceLocater::luaManager = nullptr;

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
