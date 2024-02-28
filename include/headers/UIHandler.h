#pragma once

#include <Entity.h>
#include <RenderHandler.h>
#include <SDL2x64/SDL.h>
#include <SDL2x64/SDL_image.h>

#include <iostream>
#include <list>
#include <string>

class UIHandler {
   private:
    std::list<std::shared_ptr<UIButton>> _ui;
    InputHandler* _input_handler;
    RenderHandler* _render_handler;

   public:
    void addUIElement(std::shared_ptr<UIButton> ui);
    void initUIHandler(InputHandler* input_handler,
                       RenderHandler* render_handler);
    bool handleUI();
};