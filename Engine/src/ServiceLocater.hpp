#pragma once
#include "SceneManager.hpp"
#include "RenderSystem.hpp"
#include "PhysicsSystem.hpp"
#include "LuaManager.hpp"

class ServiceLocater {
public:
	static void registerSceneManager(SceneManager* sm);
	static void registerRenderSystem(RenderSystem* rs);
	static void registerPhysicsSystem(PhysicsSystem* ps);
	static void registerLuaManager(LuaManager* lm);

	static SceneManager* getSceneManager();
	static RenderSystem* getRenderSystem();
	static PhysicsSystem* getPhysicsSystem();
	static LuaManager* getLuaManager();
private:
	static SceneManager* sceneManager;
	static RenderSystem* renderSystem;
	static PhysicsSystem* physicsSystem;
	static LuaManager* luaManager;
};