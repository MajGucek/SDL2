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

#include <time.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>

enum class GameState {
    MAIN_MENU,
    LEADERBOARD,
    SETTINGS,
    PLAY,
    DEATH,
    EXIT,
    LOGIN,
    REPLAY,
};

class Game {
   private:
    Game();
    // Handlers //
    RenderHandler _render_handler;
    InputHandler _input_handler;
    CollisionHandler _collision_handler;
    // --Handlers-- //

    // Timers //
    InternalTimer _load_timer;
    // --Timers-- //

    // Game constants //
    GameState _game_state;
    int _screen_width, _screen_height, _hp, _level, _score, _volume;
    std::string _player_name;
    // --Game constants-- //

    // Core //
    void init();
    void gameLoop();
    void startMenuLoop();
    void deathMenuLoop();
    void settingsMenuLoop();
    void gameplayLoop();
    void loginMenuLoop();
    void leaderboardMenuLoop();
    void pauseMenuLoop();
    void replayLoop();
    // --Core-- //

   public:
    ~Game();
    void run();
    static Game& getInstance();
};
