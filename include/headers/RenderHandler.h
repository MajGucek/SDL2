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
};

enum class TextureType {
    background,
    player,
    player_hit,
    hit,
    laboratory,
    laboratory_hit,
    poacher,
    score_0,
    score_1,
    score_2,
    score_3,
    score_4,
    score_5,
    score_6,
    score_7,
    score_8,
    score_9,
    start,
    start_hovered,
    settings,
    settings_hovered,
    exit,
    exit_hovered,
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