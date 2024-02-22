#include <CollisionHandler.h>

Collider::Collider(SDL_Rect* collision_hitbox) : _collider(collision_hitbox) {}
SDL_Rect* Collider::getHitbox() { return _collider; }

void CollisionHandler::setSize(int w, int h) {
    _screen_width = w;
    _screen_height = h;
}

void CollisionHandler::addCollider(std::shared_ptr<Collider> collider) {
    _colliders.push_back(std::move(collider));
}

bool CollisionHandler::isColliding(std::shared_ptr<Collider> collider) {
    if (_screen_width == 0 or _screen_height == 0) {
        std::cout << "missing size!" << std::endl;
        return true;
    }
    for (auto other_collider : _colliders) {
        SDL_Rect* hitbox = collider->getHitbox();
        if (hitbox->x < 0 or hitbox->y < 0 or
            hitbox->x + hitbox->w > _screen_width or
            hitbox->y + hitbox->h > _screen_height) {
            return true;
        }
        if (SDL_HasIntersection(other_collider->getHitbox(),
                                collider->getHitbox())) {
            return true;
        }
    }
    return false;
}

std::unique_ptr<Collider> ColliderFactory::createCollider(
    SDL_Rect* collision_hitbox) {
    auto col = std::make_unique<Collider>(collision_hitbox);
    return std::move(col);
}
