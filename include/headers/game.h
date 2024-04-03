#pragma once

// ---- //
#include <AudioHandler.h>
#include <CollisionHandler.h>
#include <EnemyHandler.h>
#include <Entity.h>
#include <RenderHandler.h>
#include <TimeHandler.h>
#include <UIHandler.h>
// ---- //

#include <iostream>
#include <memory>

enum class GameState { MAIN_MENU, SETTINGS, PLAY, DEATH, EXIT };

class Game {
   private:
    Game();
    // Handlers //
    RenderHandler _render_handler;
    InputHandler _input_handler;
    CollisionHandler _collision_handler;
    LaboratoryHandler _laboratory_handler;
    PoacherHandler _poacher_handler;
    PlayerHandler _player_handler;
    // --Handlers-- //

    // UI //
    // --UI--//

    // GameObjects //
    std::shared_ptr<Player> _player;
    // --GameObjects-- //

    // Game constants //
    int _screen_width;
    int _screen_height;
    GameState _game_state;
    // --Game constants-- //

    // Core //
    void init();
    void gameLoop();
    void startMenuLoop();
    void deathMenuLoop();
    void settingsMenuLoop();
    void gameplayLoop();
    // --Core-- //

   public:
    ~Game();
    void run();
    static Game& getInstance();
};
