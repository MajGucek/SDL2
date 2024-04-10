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
    std::vector<GameObject *> _entities;
    std::vector<std::pair<std::unique_ptr<GameObject>, int>>
        _animation_entities;
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    int _w, _h;
    static int _r_w, _r_h;
    SDL_Rect _background;

   public:
    RenderHandler();
    ~RenderHandler();
    RenderHandler &initSystem();
    RenderHandler &createWindow(int screen_width, int screen_height);
    RenderHandler &createRenderer(int index = -1, Uint32 flags = 0);

    SDL_Renderer *getRenderer();
    void includeInRender(GameObject *entity);
    void includeInRender(std::unique_ptr<GameObject> entity, int frames);
    void render();
    void clearRenderQueue();
    void clearAnimationQueue();
    void setRenderingSize(int w, int h);
    std::pair<int, int> getRenderingSize();
    static std::pair<int, int> getSize();
};

enum class TextureType {
    // width-height
    background,           // 4000x4000
    player,               // 100x100
    player_hit,           // 100x100
    hit,                  // 100x100
    laboratory,           // 200x200
    laboratory_hit,       // 200x200
    poacher,              // 100x100
    poacher_hit,          // 100x100
    pangolin,             // 300x300
    pangolin_active,      // 300x300
    ground_rune,          // 1000x1000
    score_0,              // 100x100
    score_1,              // 100x100
    score_2,              // 100x100
    score_3,              // 100x100
    score_4,              // 100x100
    score_5,              // 100x100
    score_6,              // 100x100
    score_7,              // 100x100
    score_8,              // 100x100
    score_9,              // 100x100
    A,                    // 100x100
    B,                    // 100x100
    C,                    // 100x100
    D,                    // 100x100
    E,                    // 100x100
    F,                    // 100x100
    G,                    // 100x100
    H,                    // 100x100
    I,                    // 100x100
    J,                    // 100x100
    K,                    // 100x100
    L,                    // 100x100
    M,                    // 100x100
    N,                    // 100x100
    O,                    // 100x100
    P,                    // 100x100
    Q,                    // 100x100
    R,                    // 100x100
    S,                    // 100x100
    T,                    // 100x100
    U,                    // 100x100
    V,                    // 100x100
    W,                    // 100x100
    X,                    // 100x100
    Y,                    // 100x100
    Z,                    // 100x100
    Unknown,              // 100x100
    start,                // 800x200
    start_hovered,        // 800x200
    play,                 // 800x200
    play_hovered,         // 800x200
    save,                 // 800x200
    save_hovered,         // 800x200
    load,                 // 800x200
    load_hovered,         // 800x200
    leaderboard,          // 400x400
    leaderboard_hovered,  // 400x400
    replay,               // 800x200
    replay_hovered,       // 800x200
    settings,             // 1000x300
    settings_hovered,     // 1000x300
    exit,                 // 800x200
    exit_hovered,         // 800x200
    restart,              // 1000x300
    restart_hovered,      // 1000x300
    confirm,              // 1000x300
    confirm_hovered,      // 1000x300
    input,                // 1000x100
    input_hovered,        // 1000x100
    input_inputing,       // 1000x100
    p2560x1440,           // 1200x200
    p2560x1440_hovered,   // 1200x200
    p1920x1080,           // 1200x200
    p1920x1080_hovered,   // 1200x200
    p1280x720,            // 1200x200
    p1280x720_hovered,    // 1200x200
    death_screen,         // 1900x300
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