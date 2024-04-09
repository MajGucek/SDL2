#pragma once

// ---- //
#include <AudioHandler.h>
#include <CollisionHandler.h>
#include <EnemyHandler.h>
#include <Entity.h>
#include <FileHandler.h>
#include <RenderHandler.h>
#include <TimeHandler.h>
#include <UIHandler.h>
// ---- //

#include <cstring>
#include <iostream>
#include <memory>

enum class GameState { MAIN_MENU, SETTINGS, PLAY, DEATH, EXIT, LOGIN };

class Game {
   private:
    Game();
    // Handlers //
    RenderHandler _render_handler;
    InputHandler _input_handler;
    CollisionHandler _collision_handler;
    EntityHandler _entity_handler;
    // --Handlers-- //

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
    void loginMenuLoop();
    void pauseMenuLoop();
    // --Core-- //

   public:
    ~Game();
    void run();
    static Game& getInstance();
};
