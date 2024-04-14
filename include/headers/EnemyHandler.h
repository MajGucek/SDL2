#pragma once

#include <Entity.h>
#include <FileHandler.h>
#include <RenderHandler.h>

class Player;
class CollisionHandler;
class Laboratory;
class Poacher;

#include <cmath>
#include <list>
#include <memory>
#include <vector>

static constexpr SDL_Rect player_pos = {1550, 850, 100, 100};

class IEnemyHandler {
   protected:
    int _seeing_distance = 200;
    const int _base_score = 100;

   public:
    virtual void includeInRender(RenderHandler& render_handler) = 0;
    virtual bool handle(Player* player, CollisionHandler& collision_handler,
                        RenderHandler* render_handler, float delta_time) = 0;
    virtual void init(CollisionHandler& collision_handler, int level) = 0;
};

class LaboratoryHandler : public IEnemyHandler {
   private:
    int _seeing_distance = 400;
    InternalTimer _hint;
    const int _hint_delay = 5000;
    std::vector<std::pair<std::shared_ptr<Laboratory>, bool>> _laboratories;
    void addLaboratory(int x, int y, CollisionHandler& collision_handler,
                       int hp, unsigned animals_stored);
    void setVisibility(int seeing_distance);

   public:
    void includeInRender(RenderHandler& render_handler) override;
    bool handle(Player* player, CollisionHandler& collision_handler,
                RenderHandler* render_handler, float delta_time) override;
    void init(CollisionHandler& collision_handler, int level) override;
};

class PoacherHandler : public IEnemyHandler {
   private:
    const int _seeing_distance = 800;
    const int _notice_distance = 1600;
    int _level = 1;
    InternalTimer _spawn_timer;
    const int _spawn_delay = 3000;
    std::list<std::shared_ptr<Poacher>> _poachers;
    void addPoacher(int x, int y, CollisionHandler* collision_handler, int hp,
                    unsigned velocity);

   public:
    void includeInRender(RenderHandler& render_handler) override;
    bool handle(Player* player, CollisionHandler& collision_handler,
                RenderHandler* render_handler, float delta_time) override;
    void init(CollisionHandler& collision_handler, int level) override;
};
class PlayerHandler {
   private:
    std::shared_ptr<Player> _player;

   public:
    void includeInRender(RenderHandler& render_handler);
    bool handle(CollisionHandler& collision_handler,
                RenderHandler* render_handler, float delta_time);
    void init(CollisionHandler& collision_handler, InputHandler& input_handler,
              int hp);
    Player* getPlayer();
    static int getSpeed();
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
    void init(CollisionHandler& collision_handler, InputHandler& input_handler,
              int hp, int level);
    std::pair<int, int> getGameInfo();
};
