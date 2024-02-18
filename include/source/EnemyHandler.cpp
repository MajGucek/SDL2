#include <EnemyHandler.h>

void LaboratoryHandler::addLaboratory(std::shared_ptr<Laboratory> laboratory) {
    _laboratories.push_back(std::move(laboratory));
}

void LaboratoryHandler::removeAllLaboratories() {
    _laboratories.erase(_laboratories.begin(), _laboratories.end());
}

void LaboratoryHandler::handleLaboratories()