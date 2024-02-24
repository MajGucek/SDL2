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

void LaboratoryHandler::handleLaboratories(SDL_Rect player,
                                           Scoreboard* scoreboard) {
    for (auto& lab : _laboratories) {
        if (lab.first->getHP() < 0) {
            std::cout << "unicil laboratorij!" << std::endl;
            (*scoreboard) += lab.first->getAnimalCount();
            _laboratories.remove(lab);

            continue;
        }
        if (lab.first->checkDistanceToPlayer(player, _seeing_distance)) {
            lab.second = true;
        } else {
            lab.second = false;
        }
    }
}

void LaboratoryHandler::setVisibility(int seeing_distance) {
    _seeing_distance = seeing_distance;
}