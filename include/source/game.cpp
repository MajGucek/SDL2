#include "game.h"

#include <headers/game.h>

Game::Game()
    : _screen_width(1920),
      _screen_height(1080),
      _game_state(GameState::MAIN_MENU),
      _hp(-1),
      _level(0),
      _score(0) {
    _render_handler.initSystem()
        .createWindow(_screen_width, _screen_height)
        .createRenderer(-1, SDL_RENDERER_PRESENTVSYNC);
    _collision_handler.setSize(RenderHandler::getSize());
    TextureHandler::getInstance().init(_render_handler.getRenderer());
}
Game::~Game() { SDL_Quit(); }

void Game::run() {
    srand(time(NULL));
    init();
    gameLoop();
}

Game& Game::getInstance() {
    static Game game;
    return game;
}

void Game::init() {}

void Game::gameLoop() {
    while (_game_state != GameState::EXIT) {
        if (_game_state == GameState::PLAY) {
            gameplayLoop();
        } else if (_game_state == GameState::MAIN_MENU) {
            startMenuLoop();
        } else if (_game_state == GameState::DEATH) {
            deathMenuLoop();
        } else if (_game_state == GameState::SETTINGS) {
            settingsMenuLoop();
        } else if (_game_state == GameState::LOGIN) {
            loginMenuLoop();
        } else if (_game_state == GameState::LEADERBOARD) {
            leaderboardMenuLoop();
        } else if (_game_state == GameState::REPLAY) {
            replayLoop();
        }
        _input_handler.deleteSubs();
        _input_handler.addDelay(100);
        _render_handler.clearAnimationQueue();
        _render_handler.clearRenderQueue();
    }
}

void Game::gameplayLoop() {
    EntityHandler _entity_handler;
    _entity_handler.init(_collision_handler, _input_handler, _hp, _level);
    while (_input_handler.handleInput()) {
        TimeHandler::getInstance().tick();
        float delta_time = TimeHandler::getInstance().deltaTime();
        auto resp = _entity_handler.handle(_collision_handler, &_render_handler,
                                           delta_time, _input_handler);
        if (resp == "save") {
            // savamo playerja
            auto player_info = _entity_handler.getGameInfo();
            FileHandler::getInstance().saveGame(
                _player_name.c_str(), player_info.first, player_info.second,
                Scoreboard::getInstance().getScore());
            _hp = player_info.first;
            _level = player_info.second;
            _score = Scoreboard::getInstance().getScore();
            _game_state = GameState::MAIN_MENU;
            return;
        } else if (resp == "death") {
            FileHandler::getInstance().saveScore(
                _player_name.c_str(), Scoreboard::getInstance().getScore());
            _game_state = GameState::DEATH;
            return;
        } else if (resp == "next") {
            auto it = _entity_handler.getGameInfo();
            _hp = it.first;
            _level++;
            return;
        } else {
            _entity_handler.includeInRender(_render_handler);
            _render_handler.render();
        }
    }
    _game_state = GameState::EXIT;
}

void Game::replayLoop() {
    auto player_pos = FileHandler::getInstance().loadPlayerPos();
    int ind = 0;
    std::shared_ptr<Player> player = EntityFactory::createPlayer(
        {0, 0, 100, 100}, 100, PlayerHandler::getSpeed());
    _input_handler.subscribe(player);
    SDL_Delay(500);
    while (_input_handler.handleInput()) {
        TimeHandler::getInstance().tick();
        auto state = _input_handler.getInput();
        if (state.find("x") != std::string::npos or
            ind > player_pos.size() - 1) {
            _game_state = GameState::MAIN_MENU;
            return;
        } else {
            player->setPos(player_pos.at(ind));
            ++ind;
            _render_handler.includeInRender(player.get());
            _render_handler.render();
        }
        SDL_Delay(10);
    }

    _game_state = GameState::EXIT;
}

void Game::startMenuLoop() {
    auto _start_menu = UIFactory::createStartMenu();
    _start_menu->init();
    _input_handler.subscribe(_start_menu);
    Scoreboard::getInstance().resetScore();
    auto res = FileHandler::getInstance().loadResolution();
    _screen_width = res.first;
    _screen_height = res.second;
    _render_handler.setRenderingSize(_screen_width, _screen_height);
    while (_input_handler.handleInput()) {
        TimeHandler::getInstance().tick();
        // while nismo exital programa
        _start_menu->setPlayerDisplay(_player_name, _hp, _level);
        auto start_menu_state = _start_menu->handleMenu(_render_handler);
        if (start_menu_state == "start") {
            // start game
            if (_player_name != "") {
                _game_state = GameState::PLAY;
                Scoreboard::getInstance().setScore(_score);
            } else {
                _game_state = GameState::LOGIN;
            }
            return;
        } else if (start_menu_state == "load" && !_load_timer.exists()) {
            auto save = FileHandler::getInstance().loadGame();
            if (save.name == "" or save.hp == 0) {
                _render_handler.includeInRender(
                    EntityFactory::createGameObject(TextureType::empty,
                                                    {0, 0, 800, 200}),
                    1000);
            } else {
                _player_name = save.name;
                _hp = save.hp;
                _level = save.level;
                _score = save.score;
            }
            _load_timer.startTimer(400);
        } else if (start_menu_state == "leaderboard") {
            _game_state = GameState::LEADERBOARD;
            return;
        } else if (start_menu_state == "exit") {
            _game_state = GameState::EXIT;
            return;
        } else if (start_menu_state == "settings") {
            _game_state = GameState::SETTINGS;
            return;
        } else {
            _start_menu->includeInRender(_render_handler);
            _render_handler.render();
        }
    }
    _game_state = GameState::EXIT;
}

void Game::leaderboardMenuLoop() {
    auto leaderboard_menu = UIFactory::createLeaderboardMenu();
    leaderboard_menu->init();
    _input_handler.subscribe(leaderboard_menu);

    while (_input_handler.handleInput()) {
        TimeHandler::getInstance().tick();
        auto resp = leaderboard_menu->handleMenu(_render_handler);
        if (resp == "exit") {
            _game_state = GameState::MAIN_MENU;
            return;
        } else {
            leaderboard_menu->includeInRender(_render_handler);
            _render_handler.render();
        }
    }
    _game_state = GameState::EXIT;
}

void Game::loginMenuLoop() {
    auto login_menu = UIFactory::createLoginMenu();
    login_menu->init();
    _input_handler.subscribe(login_menu);

    while (_input_handler.handleInput()) {
        TimeHandler::getInstance().tick();
        auto login_menu_state = login_menu->handleMenu(_render_handler);
        if (login_menu_state == "confirm" and login_menu->getName() != "") {
            auto player_name = login_menu->getName();
            _player_name = player_name;
            _level = 1;
            _hp = 100;
            FileHandler::getInstance().saveGame(
                player_name.c_str(), _hp, _level,
                Scoreboard::getInstance().getScore());
            _game_state = GameState::MAIN_MENU;
            return;
        } else if (login_menu_state == "exit") {
            _game_state = GameState::MAIN_MENU;
            return;
        } else {
            login_menu->includeInRender(_render_handler);
            _render_handler.render();
        }
    }

    _game_state = GameState::EXIT;
}

void Game::settingsMenuLoop() {
    auto _settings_menu = UIFactory::createSettingsMenu();
    _settings_menu->init();
    _input_handler.subscribe(_settings_menu);

    while (_input_handler.handleInput()) {
        TimeHandler::getInstance().tick();
        auto settings_menu_state = _settings_menu->handleMenu(_render_handler);
        if (settings_menu_state == "exit") {
            _game_state = GameState::MAIN_MENU;
            return;
        } else {
            _settings_menu->includeInRender(_render_handler);
            _render_handler.render();
        }
    }
    _game_state = GameState::EXIT;
}

void Game::deathMenuLoop() {
    AudioHandler::getInstance().stopSFX();
    auto _death_menu = UIFactory::createDeathMenu();
    _death_menu->playDeathAnimation(_render_handler);
    _death_menu->init();
    _input_handler.subscribe(_death_menu);
    FileHandler::getInstance().saveScore(_player_name.c_str(),
                                         Scoreboard::getInstance().getScore());
    Scoreboard::getInstance().resetScore();
    FileHandler::getInstance().saveGame(_player_name.c_str(), 0, _level,
                                        Scoreboard::getInstance().getScore());

    _hp = -1;
    _level = 0;
    _player_name = "";
    _score = 0;

    while (_input_handler.handleInput()) {
        TimeHandler::getInstance().tick();
        // if nismo exital programa
        std::string death_menu_state = _death_menu->handleMenu(_render_handler);
        if (death_menu_state == "restart") {
            // restart game
            _game_state = GameState::MAIN_MENU;
            return;
        } else if (death_menu_state == "replay") {
            _game_state = GameState::REPLAY;
            return;
        } else if (death_menu_state == "exit") {
            // exit game
            _game_state = GameState::EXIT;
            return;
        } else {
            // just render, user has not pressed anything
            _death_menu->includeInRender(_render_handler);
            _render_handler.render();
        }
    }
    // closed by input handler
    _game_state = GameState::EXIT;
}

void Game::pauseMenuLoop() {
    auto _pause_menu = UIFactory::createPauseMenu();
    _pause_menu->init();
    _input_handler.subscribe(_pause_menu);

    while (_input_handler.handleInput()) {
        TimeHandler::getInstance().tick();
        auto pause_menu_state = _pause_menu->handleMenu(_render_handler);
        if (pause_menu_state == "play") {
            return;
        } else if (pause_menu_state == "save") {
            // save to file
        } else if (pause_menu_state == "exit") {
            _game_state = GameState::MAIN_MENU;
            _input_handler.addDelay(100);
            return;
        } else {
            _pause_menu->includeInRender(_render_handler);
            _render_handler.render();
        }
    }

    _game_state = GameState::EXIT;
}
