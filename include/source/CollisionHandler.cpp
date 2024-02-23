#include <CollisionHandler.h>

void CollisionHandler::setSize(int w, int h) {
    _screen_width = w;
    _screen_height = h;
}

void CollisionHandler::addCollider(SDL_Rect* collider) {
    if (collider) {
        _colliders.push_back(collider);
    } else {
        std::cout << "collider does not exist" << std::endl;
    }
}

bool CollisionHandler::isColliding(SDL_Rect* collider) {
    if (_screen_width == 0 or _screen_height == 0) {
        std::cout << "missing size!" << std::endl;
        return true;
    }
    for (auto other_collider : _colliders) {
        if (collider->x < 0 or collider->y < 0 or
            collider->x + collider->w > _screen_width or
            collider->y + collider->h > _screen_height) {
            return true;
        }
        if (SDL_HasIntersection(other_collider, collider)) {
            return true;
        }
    }
    return false;
}
