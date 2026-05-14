#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <string_view>

class Entity {
public:
	Entity(
		std::string tag, 
		int refenceIndex,
		std::string filepath, 
		const SDL_FRect& src, 
		const SDL_FRect& dst
	);
	~Entity();
	void Init(SDL_Renderer* sdlRenderer);
	void destroy();
	bool present();
	void Update(float dt);
	void PlayAnimation(std::string animationName, bool direction);
	void Collisions(std::string tag, SDL_FRect overlap);
	bool isInitialized() const;
	void setRenderLayer(unsigned char z);
	SDL_FRect* getDstRect();
	std::string getTag() const;
	unsigned char getRenderLayer() const;
	unsigned int getId() const;
	SDL_FRect getHitbox() const;

protected:
	std::string filepath;
	SDL_Renderer* renderer;
	SDL_Texture* texture;

	SDL_FRect srcrect;
	SDL_FRect dstrect;
	SDL_FRect hitbox;

	int animationRow = 0;
	int animationFrameCount = 0;
	int animationCurrentFrame = 0;
	int animationFPS = 0;
	bool animationLoop = true;
	bool mirroredH = false;
	bool mirroredV = false;
	bool visible = true;
	double angle = 0;
	float animationTimer = 0;

	std::string tag;
	unsigned char renderLayer;
	bool isStatic;
	bool initialized = false;

	unsigned int id;
	int referenceIndex;
	static inline unsigned int nextId = 0;
};