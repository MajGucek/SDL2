#pragma once

#include <Entity.h>
#include <RenderHandler.h>

class Player;
class CollisionHandler;
class Laboratory;
class Poacher;
#include <cmath>
#include <list>
#include <memory>

class EnemyHandler {
   protected:
    int _seeing_distance = 200;

   public:
    virtual void includeInRender(RenderHandler& render_handler) = 0;
    virtual void handle(std::shared_ptr<Player> player,
                        CollisionHandler& collision_handler,
                        RenderHandler* render_handler, float delta_time) = 0;
};

class PlayerHandler {
   public:
    bool handlePlayer(std::shared_ptr<Player> player,
                      RenderHandler* render_handler, float delta_time);
};

class LaboratoryHandler : public EnemyHandler {
   private:
    std::list<std::pair<std::shared_ptr<Laboratory>, bool>> _laboratories;

   public:
    void setVisibility(int seeing_distance);
    void addLaboratory(int x, int y, CollisionHandler* collision_handler,
                       int hp, unsigned animals_stored);

    void includeInRender(RenderHandler& render_handler) override;
    void handle(std::shared_ptr<Player> player,
                CollisionHandler& collision_handler,
                RenderHandler* render_handler, float delta_time) override;
};

class PoacherHandler : public EnemyHandler {
   private:
    std::list<std::shared_ptr<Poacher>> _poachers;
    const int _seeing_distance = 800;

   public:
    void addPoacher(int x, int y, CollisionHandler* collision_handler, int hp,
                    unsigned velocity);
    void includeInRender(RenderHandler& render_handler) override;
    void handle(std::shared_ptr<Player> player,
                CollisionHandler& collision_handler,
                RenderHandler* render_handler, float delta_time) override;
};
