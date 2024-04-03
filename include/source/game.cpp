#include <headers/game.h>

Game::Game()
    : _screen_width(1920),
      _screen_height(1080),
      _game_state(GameState::MAIN_MENU) {
    _render_handler.initSystem()
        .createWindow(1920, 1080)
        .createRenderer(-1, SDL_RENDERER_PRESENTVSYNC);
    _collision_handler.setSize(_screen_width, _screen_height);
    TextureHandler::getInstance().init(_render_handler.getRenderer());
}
Game::~Game() { SDL_Quit(); }

void Game::run() {
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
        auto size = _render_handler.getSize();
        _screen_width = size.first;
        _screen_height = size.second;
        if (_game_state == GameState::PLAY) {
            gameplayLoop();
        } else if (_game_state == GameState::MAIN_MENU) {
            startMenuLoop();
        } else if (_game_state == GameState::DEATH) {
            deathMenuLoop();
        } else if (_game_state == GameState::SETTINGS) {
            settingsMenuLoop();
        }
        _input_handler.deleteSubs();
    }
}

void Game::gameplayLoop() {
    // create player
    _player = EntityFactory::createPlayer({200, 200, 100, 100}, 1, 2);
    _input_handler.subscribe(_player);
    _player->addCollisionHandler(&_collision_handler);
    _laboratory_handler.init(_collision_handler);
    _poacher_handler.init(_collision_handler);

    while (_input_handler.handleInput()) {
        TimeHandler::getInstance().tick();
        float delta_time = TimeHandler::getInstance().deltaTime();
        if (_player_handler.handlePlayer(_player, &_render_handler,
                                         delta_time)) {
            _laboratory_handler.handle(_player, _collision_handler,
                                       &_render_handler, delta_time);
            _poacher_handler.handle(_player, _collision_handler,
                                    &_render_handler, delta_time);
            _laboratory_handler.includeInRender(_render_handler);
            _poacher_handler.includeInRender(_render_handler);
            _render_handler.includeInRender(_player.get());
            _render_handler.render();
        } else {
            _game_state = GameState::DEATH;
            return;
        }
    }

    _game_state = GameState::EXIT;
}

void Game::settingsMenuLoop() {
    std::shared_ptr<SettingsMenu> _settings_menu =
        UIFactory::createSettingsMenu();
    _settings_menu->init(_screen_width, _screen_height);
    _input_handler.subscribe(_settings_menu);

    while (_input_handler.handleInput()) {
        TimeHandler::getInstance().tick();
        std::string settings_menu_state =
            _settings_menu->handleMenu(_render_handler);
        if (settings_menu_state == "exit") {
            // finished with menu
            _game_state = GameState::MAIN_MENU;
            _input_handler.addDelay(100);
            return;
        } else {
            _settings_menu->includeInRender(_render_handler);
            _render_handler.render();
        }
    }
    // input handler close
    _game_state = GameState::EXIT;
}

void Game::startMenuLoop() {
    AudioHandler::getInstance().stopSFX();
    // AudioHandler::getInstance().playSFX("my_heart");
    std::shared_ptr<StartMenu> _start_menu = UIFactory::createStartMenu();
    _start_menu->init(_screen_width, _screen_height);
    _input_handler.subscribe(_start_menu);
    //
    while (_input_handler.handleInput()) {
        TimeHandler::getInstance().tick();
        // while nismo exital programa
        std::string start_menu_state = _start_menu->handleMenu(_render_handler);
        if (start_menu_state == "start") {
            // start game
            _game_state = GameState::PLAY;
            return;
        } else if (start_menu_state == "exit") {
            // exit game
            _game_state = GameState::EXIT;
            return;
        } else if (start_menu_state == "settings") {
            // open settings menu
            _game_state = GameState::SETTINGS;
            return;
        } else {
            // just render, user has not pressed anything
            _start_menu->includeInRender(_render_handler);
            _render_handler.render();
        }
    }
    _game_state = GameState::EXIT;
}

void Game::deathMenuLoop() {
    AudioHandler::getInstance().stopSFX();
    std::shared_ptr<DeathMenu> _death_menu = UIFactory::createDeathMenu();
    _death_menu->playDeathAnimation(_screen_width, _screen_height,
                                    _render_handler);
    _death_menu->init(_screen_width, _screen_height);
    _input_handler.subscribe(_death_menu);

    while (_input_handler.handleInput()) {
        TimeHandler::getInstance().tick();
        // if nismo exital programa
        std::string death_menu_state = _death_menu->handleMenu(_render_handler);
        if (death_menu_state == "restart") {
            // restart game
            _game_state = GameState::MAIN_MENU;
            _input_handler.addDelay(100);
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
