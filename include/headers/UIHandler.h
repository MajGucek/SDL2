#pragma once

#include <Entity.h>
#include <TimeHandler.h>

#include <memory>
#include <unordered_map>
#include <vector>

class UIHandler : public InputListener {
   protected:
    InternalTimer _navigation_timer;
    int _input_delay = 100;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> _ui;
    bool _finished = false;
    bool _close_game = false;

   public:
    bool handleInput(const std::string message) override;
    virtual void init(int w, int h) = 0;
    virtual std::string handleMenu(RenderHandler& render_handler) = 0;
    virtual void includeInRender(RenderHandler& render_handler);
};

class StartMenu : public UIHandler {
   private:
    enum StartStates { Start, Settings, Exit };
    StartStates _state = StartStates::Start;

   public:
    void init(int w, int h) override;
    std::string handleMenu(RenderHandler& render_handler) override;
};

class DeathMenu : public UIHandler {
   private:
    InternalTimer _death_animation_timer;
    const int _death_animation_lenght = 4000;
    enum DeathStates {
        Restart,
        Exit,
        Save,
        Replay
    } _state = DeathStates::Restart;

   public:
    void init(int w, int h) override;
    std::string handleMenu(RenderHandler& render_handler) override;
    void playDeathAnimation(int w, int h, RenderHandler& render_handler);
};

class SettingsMenu : public UIHandler {
   private:
    enum Resolutions {
        p1280x720,
        p1920x1080,
        p2560x1440,
    } _resolution = Resolutions::p1920x1080;
    enum SettingsState { Resolution, Exit } _state = SettingsState::Resolution;

   public:
    void init(int w, int h) override;
    std::string handleMenu(RenderHandler& render_handler) override;
};

class PauseMenu : public UIHandler {
   private:
    enum PauseState { Play, Save, Exit } _state = PauseState::Play;

   public:
    void init(int w, int h) override;
    std::string handleMenu(RenderHandler& render_handler) override;
};

class UIFactory {
   public:
    static std::unique_ptr<StartMenu> createStartMenu();
    static std::unique_ptr<DeathMenu> createDeathMenu();
    static std::unique_ptr<SettingsMenu> createSettingsMenu();
    static std::unique_ptr<PauseMenu> createPauseMenu();
};