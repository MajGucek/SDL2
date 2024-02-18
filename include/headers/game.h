#pragma once

#include <CollisionHandler.h>
#include <SDL2x64/SDL.h>
#include <SDL2x64/SDL_image.h>
#include <headers/Entity.h>
#include <headers/InputHandler.h>
#include <headers/RenderHandler.h>

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

enum class GameState { PLAY, EXIT };

class Game {
   private:
    RenderHandler _render_handler;
    InputHandler _input_handler;

    std::shared_ptr<CollisionHandler> _collision_handler;

    std::shared_ptr<Entity> _background;
    std::shared_ptr<Player> _player;
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
