#include <EnemyHandler.h>

void LaboratoryHandler::addLaboratory(SDL_Renderer* ren, int x, int y,
                                      CollisionHandler* collision_handler,
                                      int hp, unsigned animals_stored) {
    std::shared_ptr<Laboratory> lab = EntityFactory::createLaboratory(
        "res/laboratory.png", ren, {x, y, 200, 200}, hp, animals_stored);
    collision_handler->addCollider(lab);
    _laboratories.push_back({std::move(lab), false});
}

void LaboratoryHandler::includeInRender(RenderHandler& render_handler) {
    for (auto& lab : _laboratories) {
        if (lab.second) {
            render_handler.includeInRender(lab.first);
        }
    }
}

void LaboratoryHandler::handle(std::shared_ptr<Player> player,
                               Scoreboard& scoreboard,
                               CollisionHandler& collision_handler,
                               RenderHandler* render_handler) {
    for (auto& lab : _laboratories) {
        if (lab.first->getHP() < 0) {
            std::cout << "unicil laboratorij!" << std::endl;
            scoreboard += lab.first->getAnimalCount();
            _laboratories.remove(lab);

            continue;
        }
        if (lab.first->checkDistanceToPlayer(*player->getHitbox(),
                                             _seeing_distance)) {
            lab.second = true;
        } else {
            lab.second = false;
        }
    }
}

void LaboratoryHandler::setVisibility(int seeing_distance) {
    _seeing_distance = seeing_distance;
}

void PoacherHandler::addPoacher(SDL_Renderer* ren, int x, int y,
                                CollisionHandler* collision_handler, int hp,
                                unsigned velocity) {
    std::shared_ptr<Poacher> poacher = EntityFactory::createPoacher(
        "res/poacher.png", ren, {x, y, 100, 100}, hp, velocity);
    poacher->addCollisionHandler(collision_handler);
    collision_handler->addCollider(poacher);
    _poachers.push_back(std::move(poacher));
}

void PoacherHandler::handle(std::shared_ptr<Player> player,
                            Scoreboard& scoreboard,
                            CollisionHandler& collision_handler,
                            RenderHandler* render_handler) {
    for (auto& poacher : _poachers) {
        if (poacher->getHP() < 0) {
            std::cout << "ubil poacherja!" << std::endl;
            scoreboard += poacher->getScore();
            _poachers.remove(poacher);

            continue;
        }
        if (poacher->canSeePlayer(*player->getHitbox(), _seeing_distance)) {
            poacher->moveTowards(*player->getHitbox());
            if (collision_handler.areColliding(*player->getHitbox(),
                                               *poacher->getHitbox())) {
                // player take damage
                if (poacher->attack(render_handler)) {
                    player->hit(poacher->getDamage(),
                                render_handler->getRenderer());
                    _poachers.remove(poacher);
                                }
            }
        }
    }
}

void PoacherHandler::includeInRender(RenderHandler& render_handler) {
    for (auto& poacher : _poachers) {
        render_handler.includeInRender(poacher);
    }
}