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
    RenderHandler _render_handler;
    InputHandler _input_handler;
    CollisionHandler _collision_handler;
    LaboratoryHandler _laboratory_handler;

    std::shared_ptr<Player> _player;
    std::shared_ptr<Entity> _background;
    std::shared_ptr<Entity> _rock;

    GameState _game_state;

    int _screen_width;
    int _screen_height;

    Uint32 _prev_time;
    float _delta_time;

    void init(const char *title, int x, int y, int w, int h, Uint32 flags);
    void gameLoop();
    void handleEvents(float delta_time);

   public:
    Game();
    ~Game();
    float getDeltaTime();

    void run();
};
