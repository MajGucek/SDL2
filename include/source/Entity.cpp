#include <headers/Entity.h>

Entity::Entity(SDL_Rect hitbox) {
    _hitbox = hitbox;
    _collider = nullptr;
}
Entity::~Entity() {
    SDL_DestroyTexture(_texture);
    _texture = nullptr;
    delete _collider;
}
void Entity::setTexture(const std::string path, SDL_Renderer* ren) {
    SDL_Surface* temp = IMG_Load(path.c_str());
    _texture = SDL_CreateTextureFromSurface(ren, temp);
    SDL_FreeSurface(temp);
}
SDL_Texture* Entity::getTexture() { return _texture; }
SDL_Rect* Entity::getHitbox() { return &_hitbox; }

void Entity::isCollider(CollisionHandler* collision_handler) {
    _collider = ColliderFactory::createCollider(&_hitbox);
    collision_handler->addCollider(_collider);
}

// Controlled Entity
ControlledEntity::~ControlledEntity() { this->~Entity(); }
ControlledEntity::ControlledEntity(SDL_Rect hitbox, unsigned velocity) {
    _hitbox = hitbox;
    _velocity = velocity;
    _collision_handler = nullptr;
}
void ControlledEntity::isCollider(CollisionHandler* collision_handler) {
    _collider = ColliderFactory::createCollider(&_hitbox);
    _collision_handler = collision_handler;
}

void Player::handleInput(const std::string message, float delta_time) {
    double move_X = 0.0, move_Y = 0.0;

    if (message.find("W") != std::string::npos) {
        move_Y -= 1.0;
    }
    if (message.find("S") != std::string::npos) {
        move_Y += 1.0;
    }
    if (message.find("A") != std::string::npos) {
        move_X -= 1.0;
    }
    if (message.find("D") != std::string::npos) {
        move_X += 1.0;
    }

    double magnitude = std::sqrt(move_X * move_X + move_Y * move_Y);
    if (magnitude > 0.0) {
        // zracuni mozne nove X, Y
        int new_x = static_cast<int>(
            _hitbox.x + (_velocity * move_X / magnitude) * delta_time);
        int new_y = static_cast<int>(
            _hitbox.y + (_velocity * move_Y / magnitude) * delta_time);

        // cekraj ce mas col_handler
        if (_collision_handler) {
            // cekiraj za X movement
            SDL_Rect tempX = {new_x, _hitbox.y, _hitbox.w, _hitbox.h};
            Collider tempColliderX =
                ColliderFactory::createColliderOnStack(&tempX);

            if (!_collision_handler->isColliding(&tempColliderX)) {
                _hitbox.x = new_x;
            }

            // cekiraj za Y movement
            SDL_Rect tempY = {_hitbox.x, new_y, _hitbox.w, _hitbox.h};
            Collider tempColliderY =
                ColliderFactory::createColliderOnStack(&tempY);

            if (!_collision_handler->isColliding(&tempColliderY)) {
                _hitbox.y = new_y;
            }
        }
    }
}
Player::Player(SDL_Rect hitbox, unsigned velocity)
    : ControlledEntity(hitbox, velocity) {}

// Factories
Entity* EntityFactory::createEntity(const std::string path, SDL_Renderer* ren,
                                    SDL_Rect hitbox) {
    auto ent = new Entity(hitbox);
    ent->setTexture(path, ren);
    return ent;
}

ControlledEntity* EntityFactory::createControlledEntity(const std::string path,
                                                        SDL_Renderer* ren,
                                                        SDL_Rect hitbox,
                                                        unsigned velocity) {
    auto cont_ent = new ControlledEntity(hitbox, velocity);
    cont_ent->setTexture(path, ren);
    return cont_ent;
}

Player* EntityFactory::createPlayer(const std::string path, SDL_Renderer* ren,
                                    SDL_Rect hitbox, unsigned velocity) {
    auto cont_ent = new Player(hitbox, velocity);
    cont_ent->setTexture(path, ren);
    return cont_ent;
}
