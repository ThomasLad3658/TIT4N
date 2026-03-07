#pragma once

class Game;
class SceneManager;
class RenderSystem;
class PhysicsSystem;
class LuaManager;

class ServiceLocater {
public:
	static void registerGame(Game* g);
	static void registerSceneManager(SceneManager* sm);
	static void registerRenderSystem(RenderSystem* rs);
	static void registerPhysicsSystem(PhysicsSystem* ps);
	static void registerLuaManager(LuaManager* lm);

	static Game* getGame();
	static SceneManager* getSceneManager();
	static RenderSystem* getRenderSystem();
	static PhysicsSystem* getPhysicsSystem();
	static LuaManager* getLuaManager();
private:
	static Game* game;
	static SceneManager* sceneManager;
	static RenderSystem* renderSystem;
	static PhysicsSystem* physicsSystem;
	static LuaManager* luaManager;
};