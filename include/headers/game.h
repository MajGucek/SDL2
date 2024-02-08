#pragma once

#include <CollisionHandler.h>
#include <SDL2x64/SDL.h>
#include <SDL2x64/SDL_image.h>
#include <headers/Entity.h>
#include <headers/InputHandler.h>

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

enum class GameState { PLAY, EXIT };

class Game {
    // remove this shit
    friend class InputHandler;

   private:
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    SDL_Surface *_surface;
    InputHandler _input_handler;
    CollisionHandler _collision_handler;

    Entity *_background;
    Player *_player;
    Entity *_rock;

    GameState _game_state;

    int _screen_width;
    int _screen_height;

    Uint32 _prev_time;
    float _delta_time;

    void init(const char *title, int x, int y, int w, int h, Uint32 flags);
    void gameLoop();
    void handleEvents(float delta_time);
    void render();

   public:
    Game();
    ~Game();
    float getDeltaTime();

    void run();
};
