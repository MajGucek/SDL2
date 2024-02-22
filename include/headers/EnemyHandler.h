#pragma once

#include <Entity.h>
#include <RenderHandler.h>

#include <cmath>
#include <list>
#include <memory>

class LaboratoryHandler {
   private:
    std::list<std::shared_ptr<Laboratory>> _laboratories;
    int _seeing_distance = 200;

   public:
    void setVisibility(int seeing_distance);
    void addLaboratory(SDL_Renderer* ren, int x, int y,
                       CollisionHandler* collision_handler);

    void includeInRender(RenderHandler& render_handler);
    void handleLaboratories(SDL_Rect player);
};
