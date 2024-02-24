#pragma once

#include <SDL2x64/SDL.h>
#include <SDL2x64/SDL_image.h>
#include <headers/Entity.h>

class Entity;
class Scoreboard;

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

class RenderHandler {
   private:
    std::list<std::shared_ptr<Entity>> _entities;
    std::list<std::pair<std::shared_ptr<Entity>, int>> _animation_entities;
    Scoreboard *_scoreboard;
    SDL_Window *_window;
    SDL_Renderer *_renderer;

   public:
    RenderHandler();
    ~RenderHandler();
    RenderHandler &initSystem();
    RenderHandler &createWindow(const char *title, int x, int y, int w, int h,
                                Uint32 flags);
    RenderHandler &createRenderer(int index = -1, Uint32 flags = 0);

    SDL_Renderer *getRenderer();
    void includeInRender(std::shared_ptr<Entity> entity);
    void includeInRender(std::shared_ptr<Entity> entity, int frames);
    void includeScoreboardInRender(Scoreboard *scoreboard);
    void render();
};