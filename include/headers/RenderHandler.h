#pragma once

#include <SDL2x64/SDL.h>
#include <SDL2x64/SDL_image.h>
#include <headers/Entity.h>

#include <iostream>
#include <list>

class RenderHandler {
   private:
    std::list<std::weak_ptr<Entity>> _entities;
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
    void includeInRender(std::weak_ptr<Entity> entity);
    void render();
};