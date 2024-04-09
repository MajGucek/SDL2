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

class IEnemyHandler {
   protected:
    int _seeing_distance = 200;

   public:
    virtual void includeInRender(RenderHandler& render_handler) = 0;
    virtual bool handle(Player* player, CollisionHandler& collision_handler,
                        RenderHandler* render_handler, float delta_time) = 0;
    virtual void init(CollisionHandler& collision_handler) = 0;
};

class LaboratoryHandler : public IEnemyHandler {
   private:
    int _seeing_distance = 400;
    std::list<std::pair<std::shared_ptr<Laboratory>, bool>> _laboratories;
    void addLaboratory(int x, int y, CollisionHandler& collision_handler,
                       int hp, unsigned animals_stored);
    void setVisibility(int seeing_distance);

   public:
    void includeInRender(RenderHandler& render_handler) override;
    bool handle(Player* player, CollisionHandler& collision_handler,
                RenderHandler* render_handler, float delta_time) override;
    void init(CollisionHandler& collision_handler) override;
};

class PoacherHandler : public IEnemyHandler {
   private:
    int _seeing_distance = 800;
    std::list<std::shared_ptr<Poacher>> _poachers;
    void addPoacher(int x, int y, CollisionHandler* collision_handler, int hp,
                    unsigned velocity);

   public:
    void includeInRender(RenderHandler& render_handler) override;
    bool handle(Player* player, CollisionHandler& collision_handler,
                RenderHandler* render_handler, float delta_time) override;
    void init(CollisionHandler& collision_handler) override;
};
class PlayerHandler {
   private:
    std::shared_ptr<Player> _player;

   public:
    void includeInRender(RenderHandler& render_handler);
    bool handle(CollisionHandler& collision_handler,
                RenderHandler* render_handler, float delta_time);
    void init(CollisionHandler& collision_handler, InputHandler& input_handler);
    Player* getPlayer();
};

class EntityHandler {
   private:
    LaboratoryHandler _lab_handler;
    PoacherHandler _poacher_handler;
    PlayerHandler _player_handler;
    InternalTimer _pause_delay;
    int _pause_time_delay;
    int _level;
    bool _over;

   public:
    EntityHandler();
    void includeInRender(RenderHandler& render_handler);
    std::string handle(CollisionHandler& collision_handler,
                       RenderHandler* render_handler, float delta_time,
                       InputHandler& input_handler);
    void init(CollisionHandler& collision_handler, InputHandler& input_handler);
    void increaseDifficulty();
};
