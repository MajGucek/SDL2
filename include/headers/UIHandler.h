#pragma once

#include <Entity.h>
#include <TimeHandler.h>

#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>

class UIHandler : public InputListener {
   protected:
    InternalTimer _navigation_timer;
    int _input_delay = 50;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> _ui;
    bool _finished = false;
    bool _close_game = false;

   public:
    bool handleInput(const std::string message) override;
    virtual void init() = 0;
    virtual std::string handleMenu(RenderHandler& render_handler) = 0;
    virtual void includeInRender(RenderHandler& render_handler);
};

class StartMenu : public UIHandler {
   private:
    enum StartStates { Start, Load, Settings, Exit };
    StartStates _state = StartStates::Start;

   public:
    void init() override;
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
    void init() override;
    std::string handleMenu(RenderHandler& render_handler) override;
    void playDeathAnimation(RenderHandler& render_handler);
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
    void init() override;
    std::string handleMenu(RenderHandler& render_handler) override;
};

class PauseMenu : public UIHandler {
   private:
    enum PauseState { Play, Save, Exit } _state = PauseState::Play;

   public:
    void init() override;
    std::string handleMenu(RenderHandler& render_handler) override;
};

class LoginMenu : public UIHandler {
   private:
    const int _name_size = 20;
    std::string _name;
    enum LoginState { Input, Confirm, Exit } _state = LoginState::Input;
    bool is_inputing = false;
    const int _char_input_delay = 100;
    InternalTimer _char_input_timer;
    InternalTimer _enter_timer;

   public:
    void init() override;
    std::string handleMenu(RenderHandler& render_handler) override;
    void includeInRender(RenderHandler& render_handler) override;
    char* getName();
};

class UIFactory {
   public:
    static std::shared_ptr<StartMenu> createStartMenu();
    static std::shared_ptr<DeathMenu> createDeathMenu();
    static std::shared_ptr<SettingsMenu> createSettingsMenu();
    static std::shared_ptr<PauseMenu> createPauseMenu();
    static std::shared_ptr<LoginMenu> createLoginMenu();
};