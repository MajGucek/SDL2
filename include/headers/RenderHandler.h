#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <TimeHandler.h>
#include <headers/Entity.h>

class GameObject;
class Scoreboard;

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

class RenderHandler {
   private:
    std::list<std::shared_ptr<GameObject>> _entities;
    std::list<std::pair<std::shared_ptr<GameObject>, int>> _animation_entities;
    Scoreboard *_scoreboard;
    SDL_Window *_window;
    SDL_Renderer *_renderer;

   public:
    RenderHandler();
    ~RenderHandler();
    RenderHandler &initSystem();
    RenderHandler &createWindow(int screen_width, int screen_height);
    RenderHandler &createRenderer(int index = -1, Uint32 flags = 0);

    SDL_Renderer *getRenderer();
    void includeInRender(std::shared_ptr<GameObject> entity);
    void includeInRender(std::shared_ptr<GameObject> entity, int frames);
    void includeScoreboardInRender(Scoreboard *scoreboard);
    void render();
};