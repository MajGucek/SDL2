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
    virtual void init(CollisionHandler& collision_handler) = 0;
};

class PlayerHandler {
   public:
    bool handlePlayer(std::shared_ptr<Player> player,
                      RenderHandler* render_handler, float delta_time);
};

class LaboratoryHandler : public EnemyHandler {
   private:
    int _seeing_distance = 400;
    std::list<std::pair<std::shared_ptr<Laboratory>, bool>> _laboratories;
    void addLaboratory(int x, int y, CollisionHandler& collision_handler,
                       int hp, unsigned animals_stored);
    void setVisibility(int seeing_distance);

   public:
    void includeInRender(RenderHandler& render_handler) override;
    void handle(std::shared_ptr<Player> player,
                CollisionHandler& collision_handler,
                RenderHandler* render_handler, float delta_time) override;
    void init(CollisionHandler& collision_handler) override;
};

class PoacherHandler : public EnemyHandler {
   private:
    int _seeing_distance = 800;
    std::list<std::shared_ptr<Poacher>> _poachers;
    void addPoacher(int x, int y, CollisionHandler* collision_handler, int hp,
                    unsigned velocity);

   public:
    void includeInRender(RenderHandler& render_handler) override;
    void handle(std::shared_ptr<Player> player,
                CollisionHandler& collision_handler,
                RenderHandler* render_handler, float delta_time) override;
    void init(CollisionHandler& collision_handler) override;
};
