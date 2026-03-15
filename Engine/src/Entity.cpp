#include <SDL3_image/SDL_image.h>
#include "Entity.hpp"

Entity::Entity(SDL_Renderer* renderer, std::string_view path, const SDL_FRect& src, const SDL_FRect& dst) :
	renderer(renderer), srcrect(src), dstrect(dst) {
	texture = IMG_LoadTexture(renderer, path.data());
	tag = "none";
	if (!texture) {
		std::cerr << "Failed to load entity texture with tag '" << tag << "' : " << SDL_GetError() << std::endl;
		throw std::runtime_error("entity texture loading failed");
	}
	renderLayer = 0;
	isStatic = true;
	hasScript = true;
	hitbox = { 0.0f, 0.0f , 0.0f , 0.0f };
}

Entity::~Entity() {
	SDL_DestroyTexture(texture);
}

void Entity::present()
{
	if (!SDL_RenderTexture(renderer, texture, &srcrect, &dstrect)) {
		std::cerr << "Failed to render entity texture with tag '" << tag << "' : " << SDL_GetError() << std::endl;
		throw std::runtime_error("entity texture rendering failed");
	}
}

void Entity::setPosition(float x, float y) {
	dstrect.x = x;
	dstrect.y = y;
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
