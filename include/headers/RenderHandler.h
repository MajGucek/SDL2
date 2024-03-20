#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <TimeHandler.h>
#include <headers/Entity.h>

class GameObject;
class Scoreboard;

#include <algorithm>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

class RenderHandler {
   private:
    std::vector<std::weak_ptr<GameObject>> _entities;
    std::vector<std::pair<std::unique_ptr<GameObject>, int>>
        _animation_entities;
    SDL_Window *_window;
    SDL_Renderer *_renderer;

   public:
    RenderHandler();
    ~RenderHandler();
    RenderHandler &initSystem();
    RenderHandler &createWindow(int screen_width, int screen_height);
    RenderHandler &createRenderer(int index = -1, Uint32 flags = 0);

    SDL_Renderer *getRenderer();
    void includeInRender(std::weak_ptr<GameObject> entity);
    void includeInRender(std::unique_ptr<GameObject> entity, int frames);
    void render();
    void clearRenderQueue();
    void clearAnimationQueue();
};

enum class TextureType {
    // width-height
    background,        // 4000x4000
    player,            // 100x100
    player_hit,        // 100x100
    hit,               // 100x100
    laboratory,        // 200x200
    laboratory_hit,    // 200x200
    poacher,           // 100x100
    poacher_hit,       // 100x100
    score_0,           // 100x100
    score_1,           // 100x100
    score_2,           // 100x100
    score_3,           // 100x100
    score_4,           // 100x100
    score_5,           // 100x100
    score_6,           // 100x100
    score_7,           // 100x100
    score_8,           // 100x100
    score_9,           // 100x100
    start,             // 800x200
    start_hovered,     // 800x200
    settings,          // 1000x300
    settings_hovered,  // 1000x300
    exit,              // 800x200
    exit_hovered,      // 800x200
    restart,           // 1000x300
    restart_hovered,   // 1000x300
    death_screen,      // 1900x300
};

class TextureHandler {
   private:
    std::unordered_map<TextureType, SDL_Texture *> _textures;
    SDL_Renderer *_renderer;
    TextureHandler();
    void loadTextures();

   public:
    ~TextureHandler();
    static TextureHandler &getInstance();
    void init(SDL_Renderer *renderer);
    SDL_Texture *getTexture(TextureType texture_type);
};