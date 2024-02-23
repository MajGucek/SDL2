#include <EnemyHandler.h>

void LaboratoryHandler::addLaboratory(SDL_Renderer* ren, int x, int y,
                                      CollisionHandler* collision_handler) {
    std::shared_ptr<Laboratory> lab = EntityFactory::createLaboratory(
        "res/laboratory.png", ren, {x, y, 200, 200});
    lab->isCollider(collision_handler);
    _laboratories.push_back({std::move(lab), false});
}

void LaboratoryHandler::includeInRender(RenderHandler& render_handler) {
    for (auto& lab : _laboratories) {
        if (lab.second) {
            render_handler.includeInRender(lab.first);
        }
    }
}

void LaboratoryHandler::handleLaboratories(SDL_Rect player) {
    for (auto& lab : _laboratories) {
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