#include <Entity.h>

GameObject::GameObject(SDL_Rect hitbox) : _hitbox(hitbox) {}
GameObject::~GameObject() {
    SDL_DestroyTexture(_texture);
    _texture = nullptr;
}
void GameObject::setTexture(const std::string path, SDL_Renderer* ren) {
    SDL_Surface* temp = IMG_Load(path.c_str());
    _texture = SDL_CreateTextureFromSurface(ren, temp);
    SDL_FreeSurface(temp);
}
SDL_Texture* GameObject::getTexture(SDL_Renderer* renderer) { return _texture; }
SDL_Rect* GameObject::getHitbox() { return &_hitbox; }

Entity::Entity(SDL_Rect hitbox, int hp) : GameObject(hitbox), _hp(hp) {}

void Entity::hit(int damage, SDL_Renderer* renderer, std::string audio) {
    if (!_invincibility_timer.exists()) {
        // ne obstaja, apply damage
        _hp -= damage;
        _invincibility_timer.startTimer(_invincibility_frames);
        if (audio != "") {
            AudioHandler::getInstance().playSFX(audio);
        }
    }
}
int Entity::getHP() { return _hp; }

// Controlled Entity
ControlledEntity::ControlledEntity(SDL_Rect hitbox, int hp, unsigned velocity)
    : Entity(hitbox, hp), _velocity(velocity), _collision_handler(nullptr) {}

void ControlledEntity::addCollisionHandler(
    CollisionHandler* collision_handler) {
    _collision_handler = collision_handler;
}
void ControlledEntity::attackUp(RenderHandler* render_handler) {}
void ControlledEntity::attackDown(RenderHandler* render_handler) {}
void ControlledEntity::attackLeft(RenderHandler* render_handler) {}
void ControlledEntity::attackRight(RenderHandler* render_handler) {}
int ControlledEntity::getDamage() { return _damage; }

Player::Player(SDL_Rect hitbox, int hp, unsigned velocity)
    : ControlledEntity(hitbox, hp, velocity) {}

void Player::hit(int damage, SDL_Renderer* renderer, std::string audio) {
    Entity::hit(damage, renderer, "damage");
    if (!_animation_timer.exists()) {
        _animation_timer.startTimer(_hit_animation_frames);
        setTexture("res/player_hit.png", renderer);
    }
}

void Player::handleInput(const std::string message, float delta_time,
                         RenderHandler* render_handler,
                         Scoreboard* scoreboard) {
    if (!_animation_timer.exists()) {
        setTexture("res/player.png", render_handler->getRenderer());
    }
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
    if (!_attack_timer.exists()) {
        // timer ne obstaja, lahko hit
        // starta timer v Player::handleHit();
        if (message.find("→") != std::string::npos) {
            hitRight(render_handler, scoreboard);
        } else if (message.find("←") != std::string::npos) {
            hitLeft(render_handler, scoreboard);
        } else if (message.find("↑") != std::string::npos) {
            hitUp(render_handler, scoreboard);
        } else if (message.find("↓") != std::string::npos) {
            hitDown(render_handler, scoreboard);
        }
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

void Player::handleHit(RenderHandler* render_handler, Scoreboard* scoreboard,
                       SDL_Rect attack_hitbox) {
    //
    _attack_timer.startTimer(_attack_frames);
    //
    if (_collision_handler) {
        auto hit = _collision_handler->isColliding(&attack_hitbox);
        if (hit.second) {
            if (auto entity = hit.first.lock()) {
                // zadel entity
                entity->hit(_damage, render_handler->getRenderer());
                // std::cout << "zadel!" << std::endl;
            }
        } else {
            // std::cout << "zgresil!" << std::endl;
        }
    }
    if (render_handler) {
        std::shared_ptr<GameObject> ent = EntityFactory::createGameObject(
            "res/hit.png", render_handler->getRenderer(), attack_hitbox);
        render_handler->includeInRender(std::move(ent), _attack_frames);
    }
}

void Player::hitUp(RenderHandler* render_handler, Scoreboard* scoreboard) {
    SDL_Rect attack_hitbox = {_hitbox.x, _hitbox.y - _hitbox.h, _hitbox.w,
                              _hitbox.h};
    handleHit(render_handler, scoreboard, attack_hitbox);
}
void Player::hitDown(RenderHandler* render_handler, Scoreboard* scoreboard) {
    SDL_Rect attack_hitbox = {_hitbox.x, _hitbox.y + _hitbox.h, _hitbox.w,
                              _hitbox.h};
    handleHit(render_handler, scoreboard, attack_hitbox);
}
void Player::hitLeft(RenderHandler* render_handler, Scoreboard* scoreboard) {
    SDL_Rect attack_hitbox = {_hitbox.x - _hitbox.w, _hitbox.y, _hitbox.w,
                              _hitbox.h};
    handleHit(render_handler, scoreboard, attack_hitbox);
}
void Player::hitRight(RenderHandler* render_handler, Scoreboard* scoreboard) {
    SDL_Rect attack_hitbox = {_hitbox.x + _hitbox.w, _hitbox.y, _hitbox.w,
                              _hitbox.h};
    handleHit(render_handler, scoreboard, attack_hitbox);
}

Laboratory::Laboratory(SDL_Rect hitbox, int hp, unsigned animals_stored)
    : Entity(hitbox, hp), _animals_stored(animals_stored) {}

SDL_Texture* Laboratory::getTexture(SDL_Renderer* renderer) {
    if (!_animation_timer.exists()) {
        setTexture("res/laboratory.png", renderer);
    }
    return _texture;
}

void Laboratory::hit(int damage, SDL_Renderer* renderer, std::string audio) {
    Entity::hit(damage, renderer, "bonk");
    // AudioHandler::getInstance().playSFX("bonk");
    _animation_timer.startTimer(_hit_animation_frames);
    setTexture("res/laboratory_hit.png", renderer);
}

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
unsigned Laboratory::getAnimalCount() { return _animals_stored; }

std::string Scoreboard::getDigitPath(int scoreDigit) {
    switch (scoreDigit) {
        case 0:
            return "res/score/0.png";
            break;
        case 1:
            return "res/score/1.png";
            break;
        case 2:
            return "res/score/2.png";
            break;
        case 3:
            return "res/score/3.png";
            break;
        case 4:
            return "res/score/4.png";
            break;
        case 5:
            return "res/score/5.png";
            break;
        case 6:
            return "res/score/6.png";
            break;
        case 7:
            return "res/score/7.png";
            break;
        case 8:
            return "res/score/8.png";
            break;
        case 9:
            return "res/score/9.png";
            break;
        default:
            std::cout << "not a digit!" << std::endl;
            break;
            return "";
    }
    return "";
}
void Scoreboard::operator++(int x) { _score++; }
void Scoreboard::operator++() { _score++; }
void Scoreboard::operator+=(unsigned inc) { _score += inc; }
void Scoreboard::operator-=(unsigned dec) { _score -= dec; }
int Scoreboard::getScore() { return _score; }
SDL_Texture* Scoreboard::getDigitTexture(int digit, SDL_Renderer* ren) {
    SDL_Surface* temp = IMG_Load(getDigitPath(digit).c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, temp);
    SDL_FreeSurface(temp);
    return texture;
}

Poacher::Poacher(SDL_Rect hitbox, int hp, unsigned velocity)
    : ControlledEntity(hitbox, hp, velocity) {}

bool Poacher::attack(RenderHandler* render_handler) {
    if (!_attack_timer.exists()) {
        // lahko attacka
        _attack_timer.startTimer(_attack_frames);
        return true;
    } else {
        // ne more attackat
        return false;
    }
}

bool Poacher::canSeePlayer(const SDL_Rect player, unsigned threshold) {
    int cent_x = _hitbox.x + _hitbox.w / 2;
    int cent_y = _hitbox.y + _hitbox.h / 2;
    int p_cent_x = player.x + player.w / 2;
    int p_cent_y = player.y + player.h / 2;
    unsigned distance = std::abs(std::sqrt(std::pow(cent_x - p_cent_x, 2) +
                                           std::pow(cent_y - p_cent_y, 2)));
    return distance < threshold;
}

void Poacher::moveTowards(const SDL_Rect destination) {
    double move_X = 0.0, move_Y = 0.0;

    if (_hitbox.x < destination.x) {
        move_X += 1.0;  // Move right
    } else if (_hitbox.x > destination.x) {
        move_X -= 1.0;  // Move left
    }

    if (_hitbox.y < destination.y) {
        move_Y += 1.0;  // Move down
    } else if (_hitbox.y > destination.y) {
        move_Y -= 1.0;  // Move up
    }

    double magnitude = std::sqrt(move_X * move_X + move_Y * move_Y);

    if (magnitude > 0.0) {
        // Calculate new X, Y coordinates
        int new_x =
            static_cast<int>(_hitbox.x + _velocity * (move_X / magnitude));
        int new_y =
            static_cast<int>(_hitbox.y + _velocity * (move_Y / magnitude));

        // Check for collisions using collision_handler
        if (_collision_handler) {
            handleCollisionHelper(new_x, new_y);
        } else {
            _hitbox.x = new_x;
            _hitbox.y = new_y;
        }
    }
}
void Poacher::handleCollisionHelper(int x, int y) {
    // Check for X movement
    SDL_Rect tempX = {x, _hitbox.y, _hitbox.w, _hitbox.h};
    if (!_collision_handler->isCollidingNotWithSelf(&tempX, &_hitbox)) {
        _hitbox.x = x;
    }

    // Check for Y movement
    SDL_Rect tempY = {_hitbox.x, y, _hitbox.w, _hitbox.h};
    if (!_collision_handler->isCollidingNotWithSelf(&tempY, &_hitbox)) {
        _hitbox.y = y;
    }
}
int Poacher::getScore() { return _score; }

// Factories
std::unique_ptr<GameObject> EntityFactory::createGameObject(
    const std::string path, SDL_Renderer* ren, SDL_Rect hitbox) {
    auto ent = std::make_unique<GameObject>(hitbox);
    ent->setTexture(path, ren);
    return std::move(ent);
}

std::unique_ptr<Entity> EntityFactory::createEntity(const std::string path,
                                                    SDL_Renderer* ren,
                                                    SDL_Rect hitbox, int hp) {
    auto ent = std::make_unique<Entity>(hitbox, hp);
    ent->setTexture(path, ren);
    return std::move(ent);
}
std::unique_ptr<Laboratory> EntityFactory::createLaboratory(
    const std::string path, SDL_Renderer* ren, SDL_Rect hitbox, int hp,
    unsigned animals_stored) {
    auto ent = std::make_unique<Laboratory>(hitbox, hp, animals_stored);
    ent->setTexture(path, ren);
    return std::move(ent);
}

std::unique_ptr<ControlledEntity> EntityFactory::createControlledEntity(
    const std::string path, SDL_Renderer* ren, SDL_Rect hitbox, int hp,
    unsigned velocity) {
    auto cont_ent = std::make_unique<ControlledEntity>(hitbox, hp, velocity);
    cont_ent->setTexture(path, ren);
    return std::move(cont_ent);
}

std::unique_ptr<Player> EntityFactory::createPlayer(const std::string path,
                                                    SDL_Renderer* ren,
                                                    SDL_Rect hitbox, int hp,
                                                    unsigned velocity) {
    auto player = std::make_unique<Player>(hitbox, hp, velocity);
    player->setTexture(path, ren);
    return std::move(player);
}

std::unique_ptr<Poacher> EntityFactory::createPoacher(const std::string path,
                                                      SDL_Renderer* ren,
                                                      SDL_Rect hitbox, int hp,
                                                      unsigned velocity) {
    auto p = std::make_unique<Poacher>(hitbox, hp, velocity);
    p->setTexture(path, ren);
    return std::move(p);
}

InputHandler::InputHandler() { _subscribers = {}; }

void InputHandler::notifySubs(std::string message, float delta_time,
                              RenderHandler* render_handler,
                              Scoreboard* scoreboard) {
    for (auto& sub : _subscribers) {
        sub->handleInput(message, delta_time, render_handler, scoreboard);
    }
}
void InputHandler::subscribe(std::shared_ptr<InputListener> observer) {
    _subscribers.push_back(std::move(observer));
}

SDL_MouseButtonEvent InputHandler::getMouse() {
    SDL_Event e;
    SDL_PollEvent(&e);
    return e.button;
}

std::string InputHandler::handleInput(float delta_time,
                                      RenderHandler* render_handler,
                                      Scoreboard* scoreboard) {
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
        notifySubs(message, delta_time, render_handler, scoreboard);
    }

    switch (e.type) {
        case SDL_QUIT:
            return "exit";
            break;
    }

    return "";
}