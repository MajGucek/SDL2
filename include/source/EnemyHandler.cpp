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

    if (!_hint.exists() and _laboratories.size() != 0) {
        int ind = (rand() % (_laboratories.size()));
        SDL_Rect hb = *_laboratories.at(ind).first->getHitbox();
        auto dot = EntityFactory::createGameObject(TextureType::hint,
                                                   {hb.x, hb.y, 200, 200});
        render_handler.includeInRender(std::move(dot), 50);
        _hint.startTimer(_hint_delay);
    }
}

bool LaboratoryHandler::handle(Player* player,
                               CollisionHandler& collision_handler,
                               RenderHandler* render_handler,
                               float delta_time) {
    auto del = std::remove_if(
        _laboratories.begin(), _laboratories.end(), [&](auto& lab) {
            if (lab.first->getHP() <= 0) {
                Scoreboard::getInstance() += lab.first->getAnimalCount();
                return true;
            } else {
                if (lab.first->checkDistanceToPlayer(*(player->getHitbox()),
                                                     _seeing_distance)) {
                    lab.second = true;
                } else {
                    lab.second = false;
                }
                return false;
            }
        });
    _laboratories.erase(del, _laboratories.end());
    if (_laboratories.size() == 0) {
        return false;
    } else {
        return true;
    }
}

void LaboratoryHandler::setVisibility(int seeing_distance) {
    _seeing_distance = seeing_distance;
}

void LaboratoryHandler::init(CollisionHandler& collision_handler, int level) {
    int w_r = rand() % (2800 + 1);
    int h_r = rand() % (1400 + 1);
    for (int i = 0; i < level * 2; ++i) {
        int hp = 200 * level;
        SDL_Rect hb = {w_r, h_r, 200, 200};
        while (collision_handler.isCollidingNotWithSelf(&hb, nullptr)) {
            w_r = rand() % (2800 + 1);
            h_r = rand() % (1400 + 1);

            hb = {w_r, h_r, 200, 200};
        }
        addLaboratory(hb.x, hb.y, collision_handler, hp,
                      (_base_score + level * 50));
    }
}

void PoacherHandler::init(CollisionHandler& collision_handler, int level) {
    _level = level;
    int w_r = rand() % (2800 + 1);
    int h_r = rand() % (1400 + 1);
    for (int i = 0; i < level * 2; ++i) {
        int hp = 10;
        SDL_Rect hb = {w_r, h_r, 100, 100};
        while (collision_handler.isCollidingNotWithSelf(&hb, nullptr) and
               !CollisionHandler::areColliding(hb, player_pos)) {
            w_r = rand() % (2800 + 1);
            h_r = rand() % (1400 + 1);

            hb = {w_r, h_r, 100, 100};
        }
        if (level > 5) {
            addPoacher(hb.x, hb.y, &collision_handler, hp, 3);
        } else {
            addPoacher(hb.x, hb.y, &collision_handler, hp, 2);
        }
    }
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
    auto del = std::remove_if(
        _poachers.begin(), _poachers.end(), [&](std::shared_ptr<Poacher>& p) {
            if (p->getHP() <= 0) {
                Scoreboard::getInstance() += p->getScore() + _level * 200;
                return true;
            } else {
                if (p->canSeePlayer(*player->getHitbox(), _seeing_distance)) {
                    p->moveTowards(*player->getHitbox(), delta_time);
                    if (collision_handler.areColliding(*player->getHitbox(),
                                                       *p->getHitbox())) {
                        // player take damage
                        if (p->canAttack(render_handler)) {
                            player->hit(p->getDamage(), render_handler);
                            return true;
                        }
                    }
                }
                return false;
            }
        });
    _poachers.erase(del, _poachers.end());
    if (!_spawn_timer.exists()) {
        int w_r = rand() % (2800 + 1);
        int h_r = rand() % (1400 + 1);
        SDL_Rect hb = {w_r, h_r, 100, 100};
        while (collision_handler.isCollidingNotWithSelf(&hb, nullptr) and
               !CollisionHandler::areColliding(hb, player_pos)) {
            w_r = rand() % (2800 + 1);
            h_r = rand() % (1400 + 1);
            hb.x = w_r;
            hb.y = h_r;
        }
        if (_level > 5) {
            addPoacher(hb.x, hb.y, &collision_handler, 10, 3);
        } else {
            addPoacher(hb.x, hb.y, &collision_handler, 10, 2);
        }

        _spawn_timer.startTimer(_spawn_delay);
    }

    if (_poachers.size() == 0) {
        return false;
    } else {
        return true;
    }
}

void PoacherHandler::includeInRender(RenderHandler& render_handler) {
    for (auto& poacher : _poachers) {
        render_handler.includeInRender(poacher.get());
    }
}

EntityHandler::EntityHandler()
    : _pause_time_delay(200), _level(1), _over(false) {}

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
    _lab_handler.init(collision_handler, level);
    _poacher_handler.init(collision_handler, level);
}

std::pair<int, int> EntityHandler::getGameInfo() {
    std::pair<int, int> info;
    info.first = _player_handler.getPlayer()->getHP();
    info.second = _level;
    return info;
}

void PlayerHandler::init(CollisionHandler& collision_handler,
                         InputHandler& input_handler, int hp) {
    _player = EntityFactory::createPlayer(player_pos, hp, 3);
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