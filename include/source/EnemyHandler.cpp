#include <EnemyHandler.h>

void LaboratoryHandler::addLaboratory(int x, int y,
                                      CollisionHandler& collision_handler,
                                      int hp, unsigned animals_stored) {
    std::shared_ptr<Laboratory> lab =
        EntityFactory::createLaboratory({x, y, 200, 200}, hp, animals_stored);
    collision_handler.addCollider(lab);
    _laboratories.push_back({std::move(lab), false});
}

void LaboratoryHandler::includeInRender(RenderHandler& render_handler) {
    for (auto& lab : _laboratories) {
        if (lab.second) {
            render_handler.includeInRender(lab.first.get());
        }
    }
}

bool LaboratoryHandler::handle(Player* player,
                               CollisionHandler& collision_handler,
                               RenderHandler* render_handler,
                               float delta_time) {
    for (auto& lab : _laboratories) {
        if (!lab.first) {
            std::cout << "lab does not exist!" << std::endl;
        }
        if (lab.first->getHP() < 0) {
            Scoreboard::getInstance() += lab.first->getAnimalCount();
            _laboratories.remove(lab);

            continue;
        }
        if (lab.first->checkDistanceToPlayer(*(player->getHitbox()),
                                             _seeing_distance)) {
            lab.second = true;
        } else {
            lab.second = false;
        }
    }
    if (_laboratories.size() == 0) {
        return false;
    }
    return true;
}

void LaboratoryHandler::setVisibility(int seeing_distance) {
    _seeing_distance = seeing_distance;
}

void LaboratoryHandler::init(CollisionHandler& collision_handler) {
    addLaboratory(700, 100, collision_handler, 100, 700);
    addLaboratory(300, 600, collision_handler, 70, 4340);
}

void PoacherHandler::init(CollisionHandler& collision_handler) {
    addPoacher(900, 900, &collision_handler, 10, 2);
}

void PoacherHandler::addPoacher(int x, int y,
                                CollisionHandler* collision_handler, int hp,
                                unsigned velocity) {
    std::shared_ptr<Poacher> poacher =
        EntityFactory::createPoacher({x, y, 100, 100}, hp, velocity);
    poacher->addCollisionHandler(collision_handler);
    collision_handler->addCollider(poacher);
    _poachers.push_back(std::move(poacher));
}

bool PoacherHandler::handle(Player* player, CollisionHandler& collision_handler,
                            RenderHandler* render_handler, float delta_time) {
    for (auto& poacher : _poachers) {
        if (poacher->getHP() < 0) {
            Scoreboard::getInstance() += poacher->getScore();
            _poachers.remove(poacher);
            continue;
        }
        if (poacher->canSeePlayer(*player->getHitbox(), _seeing_distance)) {
            poacher->moveTowards(*player->getHitbox(), delta_time);
            if (collision_handler.areColliding(*player->getHitbox(),
                                               *poacher->getHitbox())) {
                // player take damage
                if (poacher->canAttack(render_handler)) {
                    player->hit(poacher->getDamage(), render_handler);
                    _poachers.remove(poacher);
                }
            }
        }
    }
    if (_poachers.size() == 0) {
        return false;
    }
    return true;
}

void PoacherHandler::includeInRender(RenderHandler& render_handler) {
    for (auto& poacher : _poachers) {
        render_handler.includeInRender(poacher.get());
    }
}

EntityHandler::EntityHandler()
    : _pause_time_delay(200), _level(0), _over(false) {}

void EntityHandler::includeInRender(RenderHandler& render_handler) {
    _lab_handler.includeInRender(render_handler);
    _poacher_handler.includeInRender(render_handler);
    _player_handler.includeInRender(render_handler);
}

std::string EntityHandler::handle(CollisionHandler& collision_handler,
                                  RenderHandler* render_handler,
                                  float delta_time,
                                  InputHandler& input_handler) {
    auto message = input_handler.getInput();
    if (message.find("x") != std::string::npos) {
        // ESC
        return "save";
    }

    bool pl =
        _player_handler.handle(collision_handler, render_handler, delta_time);

    if (!pl) {
        return "death";
    }

    bool l = _lab_handler.handle(_player_handler.getPlayer(), collision_handler,
                                 render_handler, delta_time);

    bool p =
        _poacher_handler.handle(_player_handler.getPlayer(), collision_handler,
                                render_handler, delta_time);
    if (!l && !p) {
        return "next";
    }

    return "";
}

void EntityHandler::init(CollisionHandler& collision_handler,
                         InputHandler& input_handler, int hp, int level) {
    _level = level;
    _player_handler.init(collision_handler, input_handler, hp);
    _lab_handler.init(collision_handler);
    _poacher_handler.init(collision_handler);
}

void EntityHandler::increaseDifficulty() { _level++; }

std::pair<int, int> EntityHandler::getGameInfo() {
    std::pair<int, int> info;
    info.first = _player_handler.getPlayer()->getHP();
    info.second = _level;
    return info;
}

void PlayerHandler::init(CollisionHandler& collision_handler,
                         InputHandler& input_handler, int hp) {
    _player = EntityFactory::createPlayer({500, 500, 100, 100}, hp, 2);
    input_handler.subscribe(_player);
    _player->addCollisionHandler(&collision_handler);
}

Player* PlayerHandler::getPlayer() { return _player.get(); }

int PlayerHandler::getSpeed() { return 2; }

void PlayerHandler::includeInRender(RenderHandler& render_handler) {
    render_handler.includeInRender(_player.get());
}
bool PlayerHandler::handle(CollisionHandler& collision_handler,
                           RenderHandler* render_handler, float delta_time) {
    if (_player->getHP() <= 0) {
        FileHandler::getInstance().writePlayerPos();
        _player.~shared_ptr();
        return false;
    } else {
        SDL_Rect p_pos = *_player->getHitbox();
        FileHandler::getInstance().savePlayerPos(p_pos.x, p_pos.y);
        _player->handle(render_handler, delta_time);
        return true;
    }
}