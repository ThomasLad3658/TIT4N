#pragma once

class Game;
class SceneManager;
class RenderSystem;
class SoundSystem;
class PhysicsSystem;
class LuaManager;

class ServiceLocator {
public:
	static void registerGame(Game* g);
	static void registerSceneManager(SceneManager* sm);
	static void registerRenderSystem(RenderSystem* rs);
	static void registerSoundSystem(SoundSystem* ss);
	static void registerPhysicsSystem(PhysicsSystem* ps);
	static void registerLuaManager(LuaManager* lm);

	static Game* getGame();
	static SceneManager* getSceneManager();
	static RenderSystem* getRenderSystem();
	static SoundSystem* getSoundSystem();
	static PhysicsSystem* getPhysicsSystem();
	static LuaManager* getLuaManager();
private:
	static Game* game;
	static SceneManager* sceneManager;
	static RenderSystem* renderSystem;
	static SoundSystem* soundSystem;
	static PhysicsSystem* physicsSystem;
	static LuaManager* luaManager;
};