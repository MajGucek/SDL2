#include <CollisionHandler.h>

void CollisionHandler::setSize(std::pair<int, int> size) {
    _screen_width = size.first;
    _screen_height = size.second;
}

void CollisionHandler::addCollider(std::weak_ptr<Entity> collider) {
    if (collider.lock()) {
        _colliders.push_back(std::move(collider));
    } else {
        std::cout << "collider does not exist" << std::endl;
    }
}

std::pair<std::weak_ptr<Entity>, bool> CollisionHandler::isColliding(
    SDL_Rect* collider) {
    std::pair<std::weak_ptr<Entity>, bool> none;
    none.second = false;

    if (_screen_width == 0 or _screen_height == 0) {
        std::cout << "missing size!" << std::endl;
        return none;
    }
    for (auto it = _colliders.begin(); it != _colliders.end(); it++) {
        if (auto oth_col = (*it).lock()) {
            if (collider->x < 0 or collider->y < 0 or
                collider->x + collider->w > _screen_width or
                collider->y + collider->h > _screen_height) {
                none.second = true;
                return none;
            }

            if (SDL_HasIntersection(oth_col->getHitbox(), collider)) {
                return {(*it), true};
            }
        } else {
            _colliders.erase(it);
        }
    }

    return none;
}

bool CollisionHandler::isCollidingNotWithSelf(SDL_Rect* collider,
                                              SDL_Rect* self) {
    if (_screen_width == 0 or _screen_height == 0) {
        std::cout << "missing size!" << std::endl;
        return false;
    }
    for (auto it = _colliders.begin(); it != _colliders.end(); it++) {
        if (auto oth_col = (*it).lock()) {
            if (collider->x < 0 or collider->y < 0 or
                collider->x + collider->w > _screen_width or
                collider->y + collider->h > _screen_height) {
                return true;
            }

            if (SDL_HasIntersection(oth_col->getHitbox(), collider)) {
                if (self != oth_col->getHitbox()) {
                    return true;
                }
            }
        } else {
            _colliders.erase(it);
        }
    }
    return false;
}

bool CollisionHandler::areColliding(SDL_Rect a, SDL_Rect b) {
    return SDL_HasIntersection(&a, &b);
}
