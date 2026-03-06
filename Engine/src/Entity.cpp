#include <SDL3_image/SDL_image.h>
#include "Entity.hpp"
#include "Common.hpp"

Entity::Entity(SDL_Renderer* renderer, std::string_view path, SDL_FRect* src, SDL_FRect* dst) :
	renderer(renderer), srcrect(*src), dstrect(*dst) {
	texture = IMG_LoadTexture(renderer, path.data());
	if (!texture) {
		std::cerr << "Failed to load entity texture with tag '" << tag << "' : " << SDL_GetError() << std::endl;
		throw std::runtime_error("entity texture loading failed");
	}
	renderLayer = 0;
	isStatic = true;
	hasScript = true;
}

Entity::~Entity() {
	SDL_DestroyTexture(texture);
}

bool Entity::present()
{
	if (!SDL_RenderTexture(renderer, texture, &srcrect, &dstrect)) {
		std::cerr << "Failed to render entity texture with tag '" << tag << "' : " << SDL_GetError() << std::endl;
		throw std::runtime_error("entity texture rendering failed");
		return false;
	}
	else return true;
}

void Entity::setPosition(float x, float y) {
	dstrect.x = x;
	dstrect.y = y;
}

SDL_FRect Entity::getDstRect() {
	return dstrect;
}

std::string Entity::getTag() {
	return tag;
}

unsigned char Entity::getRenderLayer() {
	return renderLayer;
}
