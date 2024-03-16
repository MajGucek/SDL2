#pragma once

#include <Entity.h>
#include <TimeHandler.h>

#include <memory>
#include <unordered_map>
#include <vector>

enum StartStates { Start, Settings, Exit };

class UIHandler : public InputListener {
   protected:
    std::unordered_map<std::string, std::shared_ptr<GameObject>> _ui;
    bool _finished = false;

   public:
    virtual void init(int w, int h, SDL_Renderer* ren) = 0;
    virtual bool isFinished() = 0;
    virtual void includeInRender(RenderHandler& render_handler) = 0;
};

class StartMenu : public UIHandler {
   private:
    InternalTimer _navigation_timer;
    int _delay = 500;
    StartStates _state = Start;

   public:
    void init(int w, int h, SDL_Renderer* ren) override;
    bool isFinished() override;
    bool handleInput(const std::string message,
                     RenderHandler* render_handler) override;
    void includeInRender(RenderHandler& render_handler) override;
};

class UIFactory {
   public:
    static std::unique_ptr<StartMenu> createStartMenu();
};