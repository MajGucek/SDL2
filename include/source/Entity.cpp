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
void Entity::hit(int damage) { _hp -= damage; }
int Entity::getHP() { return _hp; }

// Controlled Entity
ControlledEntity::ControlledEntity(SDL_Rect hitbox, unsigned velocity) {
    _hitbox = hitbox;
    _velocity = velocity;
    _collision_handler = nullptr;
}

void ControlledEntity::addCollisionHandler(
    CollisionHandler* collision_handler) {
    _collision_handler = collision_handler;
}
void ControlledEntity::hitUp(RenderHandler* render_handler) {}
void ControlledEntity::hitDown(RenderHandler* render_handler) {}
void ControlledEntity::hitLeft(RenderHandler* render_handler) {}
void ControlledEntity::hitRight(RenderHandler* render_handler) {}

void Player::handleInput(const std::string message, float delta_time,
                         RenderHandler* render_handler) {
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
    if (message.find("→") != std::string::npos) {
        hitRight(render_handler);
    } else if (message.find("←") != std::string::npos) {
        hitLeft(render_handler);
    } else if (message.find("↑") != std::string::npos) {
        hitUp(render_handler);
    } else if (message.find("↓") != std::string::npos) {
        hitDown(render_handler);
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

            if (!_collision_handler->isColliding(&tempX).second) {
                _hitbox.x = new_x;
            }

            // cekiraj za Y movement
            SDL_Rect tempY = {_hitbox.x, new_y, _hitbox.w, _hitbox.h};

            if (!_collision_handler->isColliding(&tempY).second) {
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

void Player::hitUp(RenderHandler* render_handler) {
    SDL_Rect attack_hitbox = {_hitbox.x, _hitbox.y - _hitbox.h, _hitbox.w,
                              _hitbox.h};

    if (_collision_handler) {
        auto hit = _collision_handler->isColliding(&attack_hitbox);
        if (hit.second) {
            if (auto entity = hit.first.lock()) {
                // zadel entity
                entity->hit(_damage);
                std::cout << "zadel!" << std::endl;
            }
        } else {
            std::cout << "zgresil!" << std::endl;
        }
    }
    if (render_handler) {
        std::shared_ptr<Entity> ent = EntityFactory::createEntity(
            "res/hit.png", render_handler->getRenderer(), attack_hitbox);
        render_handler->includeInRender(std::move(ent), 14);
    } else {
        std::cout << "no render handler!" << std::endl;
    }
}
void Player::hitDown(RenderHandler* render_handler) {
    SDL_Rect attack_hitbox = {_hitbox.x, _hitbox.y + _hitbox.h, _hitbox.w,
                              _hitbox.h};
    if (_collision_handler) {
        auto hit = _collision_handler->isColliding(&attack_hitbox).first;
        if (auto entity = hit.lock()) {
            // zadel entity
            entity->hit(_damage);
            std::cout << "zadel!" << std::endl;
        } else {
            std::cout << "zgresil!" << std::endl;
        }
    }
}
void Player::hitLeft(RenderHandler* render_handler) {
    SDL_Rect attack_hitbox = {_hitbox.x - _hitbox.w, _hitbox.y, _hitbox.w,
                              _hitbox.h};
    if (_collision_handler) {
        auto hit = _collision_handler->isColliding(&attack_hitbox).first;
        if (auto entity = hit.lock()) {
            // zadel entity
            entity->hit(_damage);
            std::cout << "zadel!" << std::endl;
        } else {
            std::cout << "zgresil!" << std::endl;
        }
    }
}
void Player::hitRight(RenderHandler* render_handler) {
    SDL_Rect attack_hitbox = {_hitbox.x + _hitbox.w, _hitbox.y, _hitbox.w,
                              _hitbox.h};
    if (_collision_handler) {
        auto hit = _collision_handler->isColliding(&attack_hitbox).first;
        if (auto entity = hit.lock()) {
            // zadel entity
            entity->hit(_damage);
            std::cout << "zadel!" << std::endl;
        } else {
            std::cout << "zgresil!" << std::endl;
        }
    }
}

Laboratory::Laboratory(SDL_Rect hitbox) : Entity(hitbox) {}
bool Laboratory::checkDistanceToPlayer(const SDL_Rect player,
                                       unsigned threshold) {
    int cent_x = _hitbox.x + _hitbox.w / 2;
    int cent_y = _hitbox.y + _hitbox.h / 2;
    int p_cent_x = player.x + player.w / 2;
    int p_cent_y = player.y + player.h / 2;
    unsigned distance = std::abs(std::sqrt(std::pow(cent_x - p_cent_x, 2) +
                                           std::pow(cent_y - p_cent_y, 2)));
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

InputHandler::InputHandler() { _subscribers = {}; }

void InputHandler::notifySubs(std::string message, float delta_time,
                              RenderHandler* render_handler) {
    for (auto& sub : _subscribers) {
        sub->handleInput(message, delta_time, render_handler);
    }
}
void InputHandler::subscribe(std::shared_ptr<InputListener> observer) {
    _subscribers.push_back(std::move(observer));
}

std::string InputHandler::handleInput(float delta_time,
                                      RenderHandler* render_handler) {
    SDL_Event e;
    SDL_PollEvent(&e);
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    std::string message = "";

    if (state[SDL_SCANCODE_ESCAPE]) {
        return "exit";
    }

    if (state[SDL_SCANCODE_A]) {
        message.append("A");
    }
    if (state[SDL_SCANCODE_D]) {
        message.append("D");
    }
    if (state[SDL_SCANCODE_W]) {
        message.append("W");
    }
    if (state[SDL_SCANCODE_S]) {
        message.append("S");
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        message.append("→");
    }
    if (state[SDL_SCANCODE_LEFT]) {
        message.append("←");
    }
    if (state[SDL_SCANCODE_UP]) {
        message.append("↑");
    }
    if (state[SDL_SCANCODE_DOWN]) {
        message.append("↓");
    }
    if (message != "") {
        notifySubs(message, delta_time, render_handler);
    }

    switch (e.type) {
        case SDL_QUIT:
            return "exit";
            break;
    }

    return "";
}