#include <CollisionHandler.h>

Collider::Collider(SDL_Rect* collision_hitbox) : _collider(collision_hitbox) {}
SDL_Rect* Collider::getHitbox() { return _collider; }

void CollisionHandler::addCollider(std::shared_ptr<Collider> collider) {
    _colliders.push_back(std::move(collider));
}

bool CollisionHandler::isColliding(std::shared_ptr<Collider> collider) {
    for (auto other_collider : _colliders) {
        if (SDL_HasIntersection(other_collider->getHitbox(),
                                collider->getHitbox())) {
            return true;
        }
    }
    return false;
}

std::unique_ptr<CollisionHandler>
CollisionHandlerFactory::createCollisionHandler() {
    auto col_handler = std::make_unique<CollisionHandler>();
    return std::move(col_handler);
}

std::unique_ptr<Collider> ColliderFactory::createCollider(
    SDL_Rect* collision_hitbox) {
    auto col = std::make_unique<Collider>(collision_hitbox);
    return std::move(col);
}
