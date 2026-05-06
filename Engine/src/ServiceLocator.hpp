#pragma once

class Game;
class SceneManager;
class RenderSystem;
class PhysicsSystem;
class LuaManager;
class InputManager;

class ServiceLocator {
public:
	static void registerGame(Game* g);
	static void registerSceneManager(SceneManager* sm);
	static void registerRenderSystem(RenderSystem* rs);
	static void registerPhysicsSystem(PhysicsSystem* ps);
	static void registerLuaManager(LuaManager* lm);
	static void registerInputManager(InputManager* im);

	static Game* getGame();
	static SceneManager* getSceneManager();
	static RenderSystem* getRenderSystem();
	static PhysicsSystem* getPhysicsSystem();
	static LuaManager* getLuaManager();
	static InputManager* getInputManager();
private:
	static Game* game;
	static SceneManager* sceneManager;
	static RenderSystem* renderSystem;
	static PhysicsSystem* physicsSystem;
	static LuaManager* luaManager;
	static InputManager* inputManager;
};