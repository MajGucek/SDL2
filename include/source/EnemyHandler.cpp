#include <EnemyHandler.h>

void LaboratoryHandler::addLaboratory(SDL_Renderer* ren, int x, int y,
                                      CollisionHandler* collision_handler) {
    std::shared_ptr<Laboratory> lab = EntityFactory::createLaboratory(
        "res/laboratory.png", ren, {x, y, 200, 200});
    lab->isCollider(collision_handler);
    _laboratories.push_back(std::move(lab));
}

void LaboratoryHandler::includeInRender(RenderHandler& render_handler) {
    for (auto& lab : _laboratories) {
        render_handler.includeInRender(lab);
    }
}

void LaboratoryHandler::handleLaboratories(SDL_Rect player) {
    for (auto& lab : _laboratories) {
        if (lab->checkDistanceToPlayer(player, _seeing_distance)) {
            std::cout << "i see player" << std::endl;
        }
    }
}

void LaboratoryHandler::setVisibility(int seeing_distance) {
    _seeing_distance = seeing_distance;
}