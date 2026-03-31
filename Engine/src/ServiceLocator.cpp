#include "ServiceLocator.hpp"
#include "Game.hpp"
#include "SceneManager.hpp"
#include "RenderSystem.hpp"
#include "PhysicsSystem.hpp"
#include "LuaManager.hpp"

Game* ServiceLocator::game = nullptr;
SceneManager* ServiceLocator::sceneManager = nullptr;
RenderSystem* ServiceLocator::renderSystem = nullptr;
SoundSystem* ServiceLocator::soundSystem = nullptr;
PhysicsSystem* ServiceLocator::physicsSystem = nullptr;
LuaManager* ServiceLocator::luaManager = nullptr;

void ServiceLocator::registerGame(Game* g) {
	game = g;
}

void ServiceLocator::registerSceneManager(SceneManager* sm) {
	sceneManager = sm;
}

void ServiceLocator::registerRenderSystem(RenderSystem* rs) {
	renderSystem = rs;
}

void ServiceLocator::registerSoundSystem(SoundSystem* ss) {
	soundSystem = ss;
}

void ServiceLocator::registerPhysicsSystem(PhysicsSystem* ps) {
	physicsSystem = ps;
}

void ServiceLocator::registerLuaManager(LuaManager* lm) {
	luaManager = lm;
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

SoundSystem* ServiceLocator::getSoundSystem() {
	return soundSystem;
}

PhysicsSystem* ServiceLocator::getPhysicsSystem() {
	return physicsSystem;
}

LuaManager* ServiceLocator::getLuaManager() {
	return luaManager;
}
