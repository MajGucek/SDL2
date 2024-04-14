#pragma once

#include <Entity.h>
#include <FileHandler.h>
#include <TimeHandler.h>

#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>

TextureType getTextureType(char);
TextureType getTextureType(int);

class Menu : public InputListener {
   protected:
    InternalTimer _navigation_timer;
    int _input_delay = 100;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> _ui;
    bool _finished = false;
    bool _close_game = false;

   public:
    bool handleInput(const std::string message) override;
    virtual void init() = 0;
    virtual std::string handleMenu(RenderHandler& render_handler) = 0;
    virtual void includeInRender(RenderHandler& render_handler);
};

class StartMenu : public Menu {
   private:
    enum StartStates { Start, Load, Leaderboard, Settings, Exit };
    StartStates _state = StartStates::Start;
    std::string _nametag;
    int _hp, _level;

   public:
    StartMenu();
    void init() override;
    std::string handleMenu(RenderHandler& render_handler) override;
    void setPlayerDisplay(std::string player_name, int hp, int level);
    void includeInRender(RenderHandler& render_handler) override;
};

class DeathMenu : public Menu {
   private:
    InternalTimer _death_animation_timer;
    const int _death_animation_lenght = 4000;
    enum DeathStates {
        Restart,
        Replay,
        Exit,
    } _state = DeathStates::Restart;

   public:
    void init() override;
    std::string handleMenu(RenderHandler& render_handler) override;
    void playDeathAnimation(RenderHandler& render_handler);
};

class SettingsMenu : public Menu {
   private:
    enum Resolutions {
        p1280x720,
        p1920x1080,
        p2560x1440,
    } _resolution = Resolutions::p1920x1080;
    enum SettingsState {
        Resolution,
        Volume,
        Exit
    } _state = SettingsState::Resolution;
    int _volume = 100;
    const int _volume_symbol_time = 25;

   public:
    void init() override;
    std::string handleMenu(RenderHandler& render_handler) override;
    void setQOL(int width, int height, int volume);
};

class PauseMenu : public Menu {
   private:
    enum PauseState { Play, Save, Exit } _state = PauseState::Play;

   public:
    void init() override;
    std::string handleMenu(RenderHandler& render_handler) override;
};

class LoginMenu : public Menu {
   private:
    const int _name_size = 20;
    std::string _name;
    enum LoginState { Input, Confirm, Exit } _state = LoginState::Input;
    bool is_inputing = false;
    const int _char_input_delay = 100;
    InternalTimer _char_input_timer;
    InternalTimer _enter_timer;
    const int _input_delay = 150;

   public:
    void init() override;
    std::string handleMenu(RenderHandler& render_handler) override;
    void includeInRender(RenderHandler& render_handler) override;
    std::string getName();
};

class LeaderboardMenu : public Menu {
   private:
    std::vector<Score> _leaderboard;

   public:
    void init() override;
    std::string handleMenu(RenderHandler& render_handler) override;
    void includeInRender(RenderHandler& render_handler) override;
};

class UIFactory {
   public:
    static std::shared_ptr<StartMenu> createStartMenu();
    static std::shared_ptr<DeathMenu> createDeathMenu();
    static std::shared_ptr<SettingsMenu> createSettingsMenu();
    static std::shared_ptr<PauseMenu> createPauseMenu();
    static std::shared_ptr<LoginMenu> createLoginMenu();
    static std::shared_ptr<LeaderboardMenu> createLeaderboardMenu();
};
