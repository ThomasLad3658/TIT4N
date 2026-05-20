#include <SDL3_image/SDL_image.h>
#include "Game.hpp"
#include "Entity.hpp"
#include "LuaManager.hpp"
#include "ServiceLocator.hpp"

Entity::Entity(std::string tag, int referenceIndex, std::string filepath, const SDL_FRect& src, const SDL_FRect& dst) :
	tag(tag), referenceIndex(referenceIndex), srcrect(src), dstrect(dst), filepath(filepath)
{
	id = nextId;
	Entity::nextId++;
	texture = nullptr;
	renderer = nullptr;
	renderLayer = 0;
	isStatic = true;
	hitbox = { 0.0f, 0.0f , 0.0f , 0.0f };
}

Entity::~Entity() {
	ServiceLocator::getLuaManager()->callFunction<void>(("/" + std::to_string(referenceIndex) + ".OnDestroy").c_str(), true);
	if (initialized == true) {
		SDL_DestroyTexture(texture);
	}
	ServiceLocator::getLuaManager()->DereferenceObj(referenceIndex);
}

void Entity::Init(SDL_Renderer* sdlRenderer) {
	renderer = sdlRenderer;
	texture = IMG_LoadTexture(renderer, filepath.c_str());
	if (!texture) {
		std::cerr << "Failed to load entity texture with tag '" << tag << "' : " << SDL_GetError() << std::endl;
		throw std::runtime_error("entity texture loading failed");
	}

	ServiceLocator::getLuaManager()->callFunction<void>(("/" + std::to_string(referenceIndex) + ".OnInit").c_str(), true);

	initialized = true;
}

void Entity::destroy() {
	ServiceLocator::getGame()->DeleteEntity(this);
}

bool Entity::isInitialized() const {
	return initialized;
}

bool Entity::present()
{
	if (initialized == false || visible == false) return false;
	SDL_FlipMode flip = SDL_FLIP_NONE;
	if (mirroredH && mirroredV) flip = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
	else if(mirroredH) flip = SDL_FLIP_HORIZONTAL;
	else if(mirroredV) flip = SDL_FLIP_VERTICAL;
	
	if (!SDL_RenderTextureRotated(renderer, texture, &srcrect, &dstrect, angle, nullptr, flip)) {
		std::cerr << "Failed to render entity texture with tag '" << tag << "' : " << SDL_GetError() << std::endl;
		throw std::runtime_error("entity texture rendering failed");
	}
	return true;
}

void Entity::Update(float dt)
{
	LuaManager* luaManager = ServiceLocator::getLuaManager();
	/*
	luaManager->SetVariable<float>("/" + std::to_string(referenceIndex) + ".srcrect.x", srcrect.x);
	luaManager->SetVariable<float>("/" + std::to_string(referenceIndex) + ".srcrect.y", srcrect.y);
	luaManager->SetVariable<float>("/" + std::to_string(referenceIndex) + ".srcrect.w", srcrect.w);
	luaManager->SetVariable<float>("/" + std::to_string(referenceIndex) + ".srcrect.h", srcrect.h);

	luaManager->SetVariable<float>("/" + std::to_string(referenceIndex) + ".x", dstrect.x);
	luaManager->SetVariable<float>("/" + std::to_string(referenceIndex) + ".y", dstrect.y);
	luaManager->SetVariable<float>("/" + std::to_string(referenceIndex) + ".w", dstrect.w);
	luaManager->SetVariable<float>("/" + std::to_string(referenceIndex) + ".h", dstrect.h);
	*/
	luaManager->callFunction<void>(("/" + std::to_string(referenceIndex) + ".OnUpdate").c_str(), true, dt);

	float dstScale = luaManager->GetVariable<float>(("/" + std::to_string(referenceIndex) + ".dstScale").c_str());
	srcrect = {
		luaManager->GetVariable<float>(("/" + std::to_string(referenceIndex) + ".srcrect.x").c_str()),
		luaManager->GetVariable<float>(("/" + std::to_string(referenceIndex) + ".srcrect.y").c_str()),
		luaManager->GetVariable<float>(("/" + std::to_string(referenceIndex) + ".srcrect.w").c_str()),
		luaManager->GetVariable<float>(("/" + std::to_string(referenceIndex) + ".srcrect.h").c_str())
	};
	dstrect = {
		luaManager->GetVariable<float>(("/" + std::to_string(referenceIndex) + ".x").c_str()),
		luaManager->GetVariable<float>(("/" + std::to_string(referenceIndex) + ".y").c_str()),
		dstScale* srcrect.w,
		dstScale* srcrect.h
	};
	hitbox = {
		luaManager->GetVariable<float>(("/" + std::to_string(referenceIndex) + ".hitbox.ox").c_str()),
		luaManager->GetVariable<float>(("/" + std::to_string(referenceIndex) + ".hitbox.oy").c_str()),
		luaManager->GetVariable<float>(("/" + std::to_string(referenceIndex) + ".hitbox.w").c_str()),
		luaManager->GetVariable<float>(("/" + std::to_string(referenceIndex) + ".hitbox.h").c_str())
	};
	angle = luaManager->GetVariable<float>(("/" + std::to_string(referenceIndex) + ".angle").c_str());
	mirroredH = luaManager->GetVariable<bool>(("/" + std::to_string(referenceIndex) + ".mirroredH").c_str());
	mirroredV = luaManager->GetVariable<bool>(("/" + std::to_string(referenceIndex) + ".mirroredV").c_str());
	renderLayer = luaManager->GetVariable<int>(("/" + std::to_string(referenceIndex) + ".z").c_str());
	visible = luaManager->GetVariable<bool>(("/" + std::to_string(referenceIndex) + ".visible").c_str());
	
	// Update animations
	if (animationFrameCount > 0) {
		animationTimer += dt;
		float frameDuration = 1.0f / animationFPS;
		while (animationTimer >= frameDuration) {
			animationTimer -= frameDuration;
			animationCurrentFrame++;
			if (animationCurrentFrame >= animationFrameCount) {
				if (animationLoop) {
					animationCurrentFrame = 0;
				}
				else {
					animationCurrentFrame = animationFrameCount - 1;
				}
			}
		}
		srcrect.x = srcrect.w * animationCurrentFrame;
		srcrect.y = srcrect.h * animationRow;
	}
}

void Entity::PlayAnimation(std::string animationName) {
	LuaManager* luaManager = ServiceLocator::getLuaManager();
	std::string animationPath = "/" + std::to_string(referenceIndex) + ".animations." + animationName;
	animationRow = luaManager->GetVariable<int>((animationPath + ".row").c_str());
	animationFrameCount = luaManager->GetVariable<int>((animationPath + ".frameCount").c_str());
	animationFPS = luaManager->GetVariable<int>((animationPath + ".fps").c_str());
	animationLoop = luaManager->GetVariable<bool>((animationPath + ".loop").c_str());
	animationCurrentFrame = 0;
	animationTimer = 0;

}

void Entity::Collisions(std::string tag, unsigned int id, SDL_FRect overlap) {
	ServiceLocator::getLuaManager()->callFunction<void>(("/" + std::to_string(referenceIndex) + ".OnCollision").c_str(), true, tag, id, overlap.x, overlap.y, overlap.w, overlap.h);
}

void Entity::setRenderLayer(unsigned char z) {
	renderLayer = z;
}

SDL_FRect* Entity::getDstRect() {
	return &dstrect;
}

std::string Entity::getTag() const {
	return tag;
}

unsigned char Entity::getRenderLayer() const {
	return renderLayer;
}

unsigned int Entity::getId() const {
	return id;
}

unsigned int Entity::getReferenceIndex() const {
	return referenceIndex;
}

SDL_FRect Entity::getHitbox() const {
	return SDL_FRect{
		dstrect.x + hitbox.x,
		dstrect.y + hitbox.y,
		hitbox.w,
		hitbox.h
	};
}