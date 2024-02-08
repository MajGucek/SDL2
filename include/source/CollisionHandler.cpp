#include <CollisionHandler.h>

Collider::Collider(SDL_Rect* collision_hitbox) : _collider(collision_hitbox) {}
SDL_Rect* Collider::getHitbox() { return _collider; }

void CollisionHandler::addCollider(Collider* collider) {
    _colliders.push_back(collider);
}

bool CollisionHandler::isColliding(Collider* collider) {
    for (Collider* other_collider : _colliders) {
        if (SDL_HasIntersection(other_collider->getHitbox(),
                                collider->getHitbox())) {
            return true;
        }
    }
    return false;
}

Collider* ColliderFactory::createCollider(SDL_Rect* collision_hitbox) {
    Collider* col = new Collider(collision_hitbox);
    return col;
}

Collider ColliderFactory::createColliderOnStack(SDL_Rect* collision_hitbox) {
    Collider col(collision_hitbox);
    return col;
}