#include <Entity.h>

Entity::Entity(SDL_Rect hitbox) { _hitbox = hitbox; }
Entity::~Entity() {
    SDL_DestroyTexture(_texture);
    _texture = nullptr;
}
void Entity::setTexture(const std::string path, SDL_Renderer* ren) {
    SDL_Surface* temp = IMG_Load(path.c_str());
    _texture = SDL_CreateTextureFromSurface(ren, temp);
    SDL_FreeSurface(temp);
}
SDL_Texture* Entity::getTexture() { return _texture; }
SDL_Rect* Entity::getHitbox() { return &_hitbox; }

void Entity::isCollider(std::shared_ptr<CollisionHandler> collision_handler) {
    _collider = ColliderFactory::createCollider(&_hitbox);
    collision_handler->addCollider(_collider);
}

// Controlled Entity
ControlledEntity::ControlledEntity(SDL_Rect hitbox, unsigned velocity) {
    _hitbox = hitbox;
    _velocity = velocity;
}
void ControlledEntity::isCollider(
    std::shared_ptr<CollisionHandler> collision_handler) {
    _collider = ColliderFactory::createCollider(&_hitbox);
    _collision_handler = std::move(collision_handler);
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
        int new_x = (_hitbox.x + ((double)_velocity * (move_X / magnitude)));
        int new_y = (_hitbox.y + ((double)_velocity * (move_Y / magnitude)));

        //  cekraj ce mas col_handler
        if (_collision_handler) {
            // cekiraj za X movement
            SDL_Rect tempX = {new_x, _hitbox.y, _hitbox.w, _hitbox.h};
            std::unique_ptr<Collider> tempColliderX =
                ColliderFactory::createCollider(&tempX);

            if (!_collision_handler->isColliding(std::move(tempColliderX))) {
                _hitbox.x = new_x;
            }

            // cekiraj za Y movement
            SDL_Rect tempY = {_hitbox.x, new_y, _hitbox.w, _hitbox.h};
            std::unique_ptr<Collider> tempColliderY =
                ColliderFactory::createCollider(&tempY);

            if (!_collision_handler->isColliding(std::move(tempColliderY))) {
                _hitbox.y = new_y;
            }
            return;
        } else {
            // nimas collision_handler
            _hitbox.x = new_x;
            _hitbox.y = new_y;
        }
    }
}
Player::Player(SDL_Rect hitbox, unsigned velocity)
    : ControlledEntity(hitbox, velocity) {}

bool Laboratory::checkDistanceToPlayer(const SDL_Rect* player,
                                       unsigned threshold) {
    int distance = std::sqrt(std::pow(_hitbox.x - player.x, 2) +
                             std::pow(_hitbox.y - player.y, 2));
    return distance < threshold;
}

// Factories
std::unique_ptr<Entity> EntityFactory::createEntity(const std::string path,
                                                    SDL_Renderer* ren,
                                                    SDL_Rect hitbox) {
    auto ent = std::make_unique<Entity>(hitbox);
    ent->setTexture(path, ren);
    return std::move(ent);
}
std::unique_ptr<Laboratory> EntityFactory::createLaboratory(
    const std::string path, SDL_Renderer* ren, SDL_Rect hitbox) {
    auto ent = std::make_unique<Laboratory>(hitbox);
    ent->setTexture(path, ren);
    return std::move(ent);
}

std::unique_ptr<ControlledEntity> EntityFactory::createControlledEntity(
    const std::string path, SDL_Renderer* ren, SDL_Rect hitbox,
    unsigned velocity) {
    auto cont_ent = std::make_unique<ControlledEntity>(hitbox, velocity);
    cont_ent->setTexture(path, ren);
    return std::move(cont_ent);
}

std::unique_ptr<Player> EntityFactory::createPlayer(const std::string path,
                                                    SDL_Renderer* ren,
                                                    SDL_Rect hitbox,
                                                    unsigned velocity) {
    auto player = std::make_unique<Player>(hitbox, velocity);
    player->setTexture(path, ren);
    return std::move(player);
}
