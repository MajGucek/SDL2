#include <Entity.h>

GameObject::GameObject(SDL_Rect hitbox) : _hitbox(hitbox) {}
void GameObject::setTexture(TextureType texture_type) {
    _texture_type = texture_type;
}
SDL_Texture* GameObject::getTexture() {
    return TextureHandler::getInstance().getTexture(_texture_type);
}
SDL_Rect* GameObject::getHitbox() { return &_hitbox; }

Entity::Entity(SDL_Rect hitbox, int hp) : GameObject(hitbox), _hp(hp) {}

void Entity::hit(int damage, RenderHandler* render_handler) {
    if (!_invincibility_timer.exists()) {
        // ne obstaja, apply damage
        _hp -= damage;
        _invincibility_timer.startTimer(_invincibility_time);
    }
}
int Entity::getHP() { return _hp; }

// Controlled Entity
ControlledEntity::ControlledEntity(SDL_Rect hitbox, int hp, int velocity)
    : Entity(hitbox, hp), _velocity(velocity), _collision_handler(nullptr) {}

void ControlledEntity::addCollisionHandler(
    CollisionHandler* collision_handler) {
    _collision_handler = collision_handler;
}

int ControlledEntity::getDamage() { return _damage; }

void ControlledEntity::setPos(std::pair<int, int> pos) {
    _hitbox.x = pos.first;
    _hitbox.y = pos.second;
}

Player::Player(SDL_Rect hitbox, int hp, int velocity)
    : ControlledEntity(hitbox, hp, velocity) {
    _texture_type = TextureType::player;
}

void Player::hit(int damage, RenderHandler* render_handler) {
    Entity::hit(damage, render_handler);
    AudioHandler::getInstance().playSFX("damage");
    if (!_animation_timer.exists()) {
        _animation_timer.startTimer(_hit_animation_time);
        setTexture(TextureType::player_hit);
    }
}

void Player::handle(RenderHandler* render_handler, float delta_time) {
    if (!_animation_timer.exists()) {
        setTexture(TextureType::player);
    }
    // bool will_move = false;
    double move_X = 0.0, move_Y = 0.0;
    if (_message.find("W") != std::string::npos) {
        move_Y -= 1.0;
        // will_move = true;
    }
    if (_message.find("S") != std::string::npos) {
        move_Y += 1.0;
        // will_move = true;
    }
    if (_message.find("A") != std::string::npos) {
        move_X -= 1.0;
        // will_move = true;
    }
    if (_message.find("D") != std::string::npos) {
        move_X += 1.0;
        // will_move = true;
    }
    if (!_attack_timer.exists()) {
        // timer ne obstaja, lahko hit
        // starta timer v Player::handleHit();
        if (_message.find("→") != std::string::npos) {
            hitRight(render_handler);
        } else if (_message.find("←") != std::string::npos) {
            hitLeft(render_handler);
        } else if (_message.find("↑") != std::string::npos) {
            hitUp(render_handler);
        } else if (_message.find("↓") != std::string::npos) {
            hitDown(render_handler);
        }
    }

    if (_message != "") {
        double magnitude = std::sqrt(move_X * move_X + move_Y * move_Y);
        if (magnitude > 0.0) {
            // zracuni mozne nove X, Y
            int new_x =
                (_hitbox.x + ((static_cast<float>(_velocity) * delta_time) *
                              (move_X / magnitude)));
            int new_y =
                (_hitbox.y + ((static_cast<float>(_velocity) * delta_time) *
                              (move_Y / magnitude)));

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

            } else {
                // nimas collision_handler
                _hitbox.x = new_x;
                _hitbox.y = new_y;
            }
        }
        _message = "";
    }
}

void Player::setHP(int hp) { _hp = hp; }

bool Player::handleInput(const std::string message) {
    _message = message;
    return true;
}

void Player::handleHit(RenderHandler* render_handler, SDL_Rect attack_hitbox) {
    //
    _attack_timer.startTimer(_attack_delay_time);
    //
    if (_collision_handler) {
        auto hit = _collision_handler->isColliding(&attack_hitbox);
        if (hit.second) {
            if (auto entity = hit.first.lock()) {
                // zadel entity
                entity->hit(_damage, render_handler);
                // std::cout << "zadel!" << std::endl;
            }
        } else {
            // std::cout << "zgresil!" << std::endl;
        }
    }
    if (render_handler) {
        auto ent =
            EntityFactory::createGameObject(TextureType::hit, attack_hitbox);
        render_handler->includeInRender(std::move(ent), _attack_animation_time);
    }
}

void Player::hitUp(RenderHandler* render_handler) {
    SDL_Rect attack_hitbox = {_hitbox.x - (_attack_size / 3),
                              _hitbox.y - _hitbox.h - (_attack_size / 3),
                              _attack_size, _attack_size};
    /*
SDL_Rect attack_hitbox = {_hitbox.x, _hitbox.y - _hitbox.h, _hitbox.w,
    _hitbox.h};
    */
    handleHit(render_handler, attack_hitbox);
}
void Player::hitDown(RenderHandler* render_handler) {
    SDL_Rect attack_hitbox = {_hitbox.x - (_attack_size / 3),
                              _hitbox.y + _hitbox.h - (_attack_size / 3),
                              _attack_size, _attack_size};
    handleHit(render_handler, attack_hitbox);
}
void Player::hitLeft(RenderHandler* render_handler) {
    SDL_Rect attack_hitbox = {_hitbox.x - _hitbox.w - (_attack_size / 3),
                              _hitbox.y - (_attack_size / 3), _attack_size,
                              _attack_size};
    handleHit(render_handler, attack_hitbox);
}
void Player::hitRight(RenderHandler* render_handler) {
    SDL_Rect attack_hitbox = {_hitbox.x + _hitbox.w - (_attack_size / 3),
                              _hitbox.y - (_attack_size / 3), _attack_size,
                              _attack_size};
    handleHit(render_handler, attack_hitbox);
}

Laboratory::Laboratory(SDL_Rect hitbox, int hp, unsigned animals_stored)
    : Entity(hitbox, hp), _animals_stored(animals_stored) {
    _texture_type = TextureType::laboratory;
}

SDL_Texture* Laboratory::getTexture() {
    if (!_animation_timer.exists()) {
        setTexture(TextureType::laboratory);
    }
    return TextureHandler::getInstance().getTexture(_texture_type);
}

void Laboratory::hit(int damage, RenderHandler* render_handler) {
    Entity::hit(damage, render_handler);
    AudioHandler::getInstance().playSFX("bonk");
    _animation_timer.startTimer(_hit_animation_time);
    setTexture(TextureType::laboratory_hit);
}

bool Laboratory::checkDistanceToPlayer(SDL_Rect player, unsigned threshold) {
    int cent_x = _hitbox.x + _hitbox.w / 2;
    int cent_y = _hitbox.y + _hitbox.h / 2;
    int p_cent_x = player.x + player.w / 2;
    int p_cent_y = player.y + player.h / 2;
    unsigned distance = std::abs(std::sqrt(std::pow(cent_x - p_cent_x, 2) +
                                           std::pow(cent_y - p_cent_y, 2)));
    return distance < threshold;
}
unsigned Laboratory::getAnimalCount() { return _animals_stored; }

Scoreboard& Scoreboard::getInstance() {
    static Scoreboard timer;
    return timer;
}

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
void Scoreboard::resetScore() { _score = 0; }
void Scoreboard::setScore(int score) { _score = score; }
int Scoreboard::getScore() { return _score; }
SDL_Texture* Scoreboard::getDigitTexture(int digit, SDL_Renderer* ren) {
    SDL_Surface* temp = IMG_Load(getDigitPath(digit).c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, temp);
    SDL_FreeSurface(temp);
    return texture;
}

Poacher::Poacher(SDL_Rect hitbox, int hp, int velocity)
    : ControlledEntity(hitbox, hp, velocity) {
    _texture_type = TextureType::poacher;
}

bool Poacher::canAttack(RenderHandler* render_handler) {
    if (!_attack_timer.exists()) {
        // lahko attacka
        _attack_timer.startTimer(_attack_delay_time);
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

void Poacher::moveTowards(const SDL_Rect destination, float delta_time) {
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

void Poacher::hit(int damage, RenderHandler* render_handler) {
    Entity::hit(damage, render_handler);
    AudioHandler::getInstance().playSFX("zombie_hit");
    auto ent =
        EntityFactory::createGameObject(TextureType::poacher_hit, _hitbox);
    render_handler->includeInRender(std::move(ent), _death_time);
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

Pangolin::Pangolin(SDL_Rect hitbox, int hp, int velocity, int damage)
    : ControlledEntity(hitbox, hp, velocity), _damage(damage) {
    _texture_type = TextureType::pangolin;
}

void Pangolin::attack(RenderHandler* render_handler) {
    SDL_Rect rune_hb = {_hitbox.x - 500, _hitbox.y - 500, 1000, 1000};

    auto damage_circle =
        EntityFactory::createGameObject(TextureType::ground_rune, rune_hb);
    render_handler->includeInRender(std::move(damage_circle), _rune_time);
}

// Factories
std::unique_ptr<GameObject> EntityFactory::createGameObject(
    TextureType texture_type, SDL_Rect hitbox) {
    auto ent = std::make_unique<GameObject>(hitbox);
    ent->setTexture(texture_type);
    return std::move(ent);
}

std::unique_ptr<ControlledEntity> EntityFactory::createGameControlledEntity(
    TextureType texture_type, SDL_Rect hitbox, int velocity) {
    auto ent = std::make_unique<ControlledEntity>(hitbox, 1, velocity);
    ent->setTexture(texture_type);
    return std::move(ent);
}

std::unique_ptr<Laboratory> EntityFactory::createLaboratory(
    SDL_Rect hitbox, int hp, unsigned animals_stored) {
    auto ent = std::make_unique<Laboratory>(hitbox, hp, animals_stored);
    return std::move(ent);
}

std::unique_ptr<Player> EntityFactory::createPlayer(SDL_Rect hitbox, int hp,
                                                    int velocity) {
    auto player = std::make_unique<Player>(hitbox, hp, velocity);
    return std::move(player);
}

std::unique_ptr<Poacher> EntityFactory::createPoacher(SDL_Rect hitbox, int hp,
                                                      int velocity) {
    auto p = std::make_unique<Poacher>(hitbox, hp, velocity);
    return std::move(p);
}

bool InputHandler::notifySubs(std::string message) {
    for (auto sub : _subscribers) {
        if (auto subscriber = sub.lock()) {
            if (!subscriber->handleInput(message)) {
                return false;
            }
        } else {
            //_subscribers.remove(sub);
        }
    }
    return true;
}
void InputHandler::subscribe(std::weak_ptr<InputListener> observer) {
    _subscribers.push_back(std::move(observer));
}

bool InputHandler::handleInput() {
    SDL_Event e;
    SDL_PollEvent(&e);
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    _message = "";

    for (int code = SDL_SCANCODE_A; code <= SDL_SCANCODE_Z; ++code) {
        if (state[code]) {
            _message += 'A' + (code - SDL_SCANCODE_A);
        }
    }
    if (state[SDL_SCANCODE_ESCAPE]) {
        _message.append("x");
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        _message.append("→");
    }
    if (state[SDL_SCANCODE_LEFT]) {
        _message.append("←");
    }
    if (state[SDL_SCANCODE_UP]) {
        _message.append("↑");
    }
    if (state[SDL_SCANCODE_DOWN]) {
        _message.append("↓");
    }
    if (state[SDL_SCANCODE_RETURN]) {
        _message.append("e");
    }
    if (state[SDL_SCANCODE_BACKSPACE]) {
        _message.append("b");
    }
    if (state[SDL_SCANCODE_SPACE]) {
        _message.append("s");
    }

    if (_message != "" && !_delay.exists()) {
        if (!notifySubs(_message)) {
            return false;
        }
    }

    switch (e.type) {
        case SDL_QUIT:
            return false;
            break;
    }

    if (_subscribers.size() == 0) {
        return false;
    }

    return true;
}

void InputHandler::addDelay(int ms) {
    if (_delay.exists()) {
        std::cout << "too fast" << std::endl;
    } else {
        _delay.startTimer(ms);
    }
}

std::string InputHandler::getInput() { return _message; }

void InputHandler::deleteSubs() { _subscribers.clear(); }

bool InputListener::handleInput(const std::string message) {
    _message = message;
    return true;
}
