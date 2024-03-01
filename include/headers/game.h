#pragma once

// ---- //
#include <Handlers.h>
// ---- //

#include <iostream>
#include <memory>

enum class GameState { PLAY, EXIT };

class Game {
   private:
    // Handlers //
    RenderHandler _render_handler;
    InputHandler _input_handler;
    CollisionHandler _collision_handler;
    LaboratoryHandler _laboratory_handler;
    PoacherHandler _poacher_handler;
    // --Handlers-- //

    // GameObjects //
    Scoreboard _scoreboard;
    std::shared_ptr<Player> _player;
    std::shared_ptr<GameObject> _background;
    // --GameObjects-- //

    // Game constants //
    int _screen_width;
    int _screen_height;
    GameState _game_state;
    // --Game constants-- //

    // Timers //
    int _fps;
    float _delta_time;
    // --Timers-- //

    // Core //
    void init();
    void gameLoop();
    // --Core-- //

    // Helper methods //
    void handleEvents(float delta_time);
    float getDeltaTime();
    void includeInRender();
    void handleEnemies();
    // --Helper methods-- //

   public:
    Game();
    ~Game();
    void run();
};
