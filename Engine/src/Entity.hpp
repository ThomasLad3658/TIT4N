#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <string>
#include <string_view>

class Entity {
public:
	Entity(int referenceIndex);
	~Entity();
	void Init(SDL_Renderer* sdlRenderer);
	void destroy();
	bool present();
	void Update(float dt);
	void PlayAnimation(std::string animationName);
	void Collisions(std::string tag, unsigned int id, SDL_FRect overlap);
	bool isInitialized() const;
	bool isVisible() const;
	void setRenderLayer(unsigned char z);
	SDL_FRect* getDstRect();
	std::string getTag() const;
	std::string getName() const;
	unsigned char getRenderLayer() const;
	unsigned int getId() const;
	unsigned int getReferenceIndex() const;
	SDL_FRect getHitbox() const;
	void CreateEntityFromEntity(std::string entityPath);

protected:
	std::string filepath;
	SDL_Renderer* renderer;
	SDL_Texture* texture;

	SDL_FRect srcrect;
	SDL_FRect dstrect;
	SDL_FRect hitbox;

	int animationRow;
	int animationFrameCount;
	int animationCurrentFrame;
	int animationFPS;
	bool animationLoop;
	bool mirroredH;
	bool mirroredV;
	bool visible;
	double angle;
	float animationTimer;

	std::string tag;
	std::string name;
	unsigned char renderLayer;
	bool initialized;

	unsigned int id;
	int referenceIndex;
	static inline unsigned int nextId = 0;
};