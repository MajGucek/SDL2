#pragma once

#include <Entity.h>

#include <list>
#include <memory>

class LaboratoryHandler {
   private:
    std::list<std::shared_ptr<Laboratory>> _laboratories;

   public:
    void addLaboratory(std::shared_ptr<Laboratory> laboratory);
    void removeAllLaboratories();
    void handleLaboratories();
};