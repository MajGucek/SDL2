#pragma once

#include <Entity.h>
#include <RenderHandler.h>

#include <cmath>
#include <list>
#include <memory>

class EnemyHandler {
   protected:
    int _seeing_distance = 200;

   public:
    virtual void includeInRender(RenderHandler& render_handler) = 0;
    virtual void handle(std::shared_ptr<Player> player, Scoreboard& scoreboard,
                        CollisionHandler& collision_handler,
                        RenderHandler* render_handler) = 0;
};

class LaboratoryHandler : public EnemyHandler {
   private:
    std::list<std::pair<std::shared_ptr<Laboratory>, bool>> _laboratories;

   public:
    void setVisibility(int seeing_distance);
    void addLaboratory(SDL_Renderer* ren, int x, int y,
                       CollisionHandler* collision_handler, int hp,
                       unsigned animals_stored);

    virtual void includeInRender(RenderHandler& render_handler) override;
    virtual void handle(std::shared_ptr<Player> player, Scoreboard& scoreboard,
                        CollisionHandler& collision_handler,
                        RenderHandler* render_handler) override;
};

class PoacherHandler : public EnemyHandler {
   private:
    std::list<std::shared_ptr<Poacher>> _poachers;
    const int _seeing_distance = 800;

   public:
    void addPoacher(SDL_Renderer* ren, int x, int y,
                    CollisionHandler* collision_handler, int hp,
                    unsigned velocity);
    virtual void includeInRender(RenderHandler& render_handler) override;
    virtual void handle(std::shared_ptr<Player> player, Scoreboard& scoreboard,
                        CollisionHandler& collision_handler,
                        RenderHandler* render_handler) override;
};
