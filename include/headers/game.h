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

enum class GameState { PLAY, DEATH, EXIT };

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
    std::shared_ptr<StartMenu> _start_menu;
    // --UI--//

    // GameObjects //
    std::shared_ptr<Player> _player;
    std::shared_ptr<GameObject> _background;
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
    // --Core-- //

    // Helper methods //
    void includeInRender();
    void handleEnemies(float deltaTime);
    // --Helper methods-- //

   public:
    ~Game();
    void run();
    void stop();
    static Game& getInstance();
};
