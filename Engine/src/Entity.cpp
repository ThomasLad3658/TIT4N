#include <SDL3_image/SDL_image.h>
#include "Entity.hpp"

Entity::Entity(std::string path, const SDL_FRect& src, const SDL_FRect& dst) :
	srcrect(src), dstrect(dst)
{
	filepath = std::string(path.data());
	id = nextId;
	Entity::nextId++;
	texture = nullptr;
	renderer = nullptr;
	tag = "none";
	renderLayer = 0;
	isStatic = true;
	hasScript = true;
	hitbox = { 0.0f, 0.0f , 0.0f , 0.0f };
}

Entity::~Entity() {
	if (initialized == true) {
		SDL_DestroyTexture(texture);
	}
}

void Entity::Init(SDL_Renderer* sdlRenderer) {
	renderer = sdlRenderer;
	texture = IMG_LoadTexture(renderer, filepath.c_str());
	if (!texture) {
		std::cerr << "Failed to load entity texture with tag '" << tag << "' : " << SDL_GetError() << std::endl;
		throw std::runtime_error("entity texture loading failed");
	}
	initialized = true;
}

bool Entity::isInitialized() const {
	return initialized;
}

bool Entity::present()
{
	if (initialized == false) return false;
	if (!SDL_RenderTexture(renderer, texture, &srcrect, &dstrect)) {
		std::cerr << "Failed to render entity texture with tag '" << tag << "' : " << SDL_GetError() << std::endl;
		throw std::runtime_error("entity texture rendering failed");
	}
	return true;
}

void Entity::setPosition(float x, float y) {
	dstrect.x = x;
	dstrect.y = y;
}

void Entity::setRenderLayer(char z) {
	renderLayer = z;
}

SDL_FRect Entity::getDstRect() const {
	return dstrect;
}

std::string Entity::getTag() const {
	return tag;
}

unsigned char Entity::getRenderLayer() const {
	return renderLayer;
}

unsigned int Entity::getId() {
	return id;
}