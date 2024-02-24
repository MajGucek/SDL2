#pragma once
// ---- //
#include <Entity.h>
#include <Handlers.h>
// ---- //
#include <SDL2x64/SDL.h>
#include <SDL2x64/SDL_image.h>
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
    // --Handlers-- //
    // GameObjects //
    Scoreboard _scoreboard;
    std::shared_ptr<Player> _player;
    std::shared_ptr<GameObject> _background;
    // --GameObjects-- //
    // Game constants //
    GameState _game_state;
    int _screen_width;
    int _screen_height;
    // --Game constants-- //
    // Timers //
    Uint32 _prev_time;
    float _delta_time;
    // --Timers-- //
    // Helper methods //
    void init(const char *title, int x, int y, int w, int h, Uint32 flags);
    void gameLoop();
    void handleEvents(float delta_time);
    float getDeltaTime();
    // --Helper methods-- //

   public:
    Game();
    ~Game();

    void run();
};
