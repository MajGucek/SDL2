#include <headers/game.h>

Game::Game()
    : _screen_width(2560), _screen_height(1440), _game_state(GameState::PLAY) {
    _render_handler.initSystem()
        .createWindow(_screen_width, _screen_height)
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

void Game::init() {
    // create background
    _background = EntityFactory::createGameObject(
        TextureType::background, {0, 0, _screen_width, _screen_height});

    startMenuLoop();

    // create player
    _player = EntityFactory::createPlayer({200, 200, 100, 100}, 1, 2);
    _input_handler.subscribe(_player);
    _player->addCollisionHandler(&_collision_handler);
    // set laboratory constants
    _laboratory_handler.setVisibility(400);
    _laboratory_handler.addLaboratory(700, 100, &_collision_handler, 100, 700);
    _laboratory_handler.addLaboratory(300, 600, &_collision_handler, 70, 4340);
    _poacher_handler.addPoacher(900, 900, &_collision_handler, 10, 2);
}

void Game::gameLoop() {
    while (_game_state != GameState::EXIT) {
        TimeHandler::getInstance().tick();
        float delta_time = TimeHandler::getInstance().deltaTime();
        if (_game_state == GameState::PLAY) {
            if (!_input_handler.handleInput()) {
                _game_state = GameState::EXIT;
                continue;
            }

            if (!_player_handler.handlePlayer(_player, &_render_handler,
                                              delta_time)) {
                _game_state = GameState::DEATH;
                continue;
            }
            handleEnemies(delta_time);

            includeInRender();
            _render_handler.render();
        } else if (_game_state == GameState::DEATH) {
            // render death
            deathMenuLoop();
        }
    }
}

void Game::deathMenuLoop() {
    _death_menu = UIFactory::createDeathMenu();
    _death_menu->playDeathAnimation(_screen_width, _screen_height,
                                    _render_handler);
    _death_menu->init(_screen_width, _screen_height);
    _input_handler.subscribe(_death_menu);

    while (_input_handler.handleInput()) {
        // while nismo exital programa
        std::string death_menu_state = _death_menu->handleMenu(_render_handler);
        if (death_menu_state == "restart") {
            // restart game
            _game_state = GameState::PLAY;
            return;
        } else if (death_menu_state == "exit") {
            // exit game
            _game_state = GameState::EXIT;
            return;
        } else {
            // just render, user has not pressed anything
            _render_handler.includeInRender(_background);
            _death_menu->includeInRender(_render_handler);
            _render_handler.render();
        }
    }

    _game_state = GameState::EXIT;
}

void Game::startMenuLoop() {
    _start_menu = UIFactory::createStartMenu();
    _start_menu->init(_screen_width, _screen_height);
    _input_handler.subscribe(_start_menu);

    while (_input_handler.handleInput()) {
        // while nismo exital programa
        std::string start_menu_state = _start_menu->handleMenu(_render_handler);
        if (start_menu_state == "start") {
            // start game
            AudioHandler::getInstance().stopSFX();
            AudioHandler::getInstance().playSFX("twinkle");
            return;
        } else if (start_menu_state == "exit") {
            // exit game
            _game_state = GameState::EXIT;
            return;
        } else if (start_menu_state == "settings") {
            // open settings menu
        } else {
            // just render, user has not pressed anything
            _render_handler.includeInRender(_background);
            _start_menu->includeInRender(_render_handler);
            _render_handler.render();
        }
    }
}

void Game::handleEnemies(float deltaTime) {
    _laboratory_handler.handle(_player, _collision_handler, &_render_handler,
                               deltaTime);
    _poacher_handler.handle(_player, _collision_handler, &_render_handler,
                            deltaTime);
}

void Game::includeInRender() {
    _render_handler.includeInRender(_background);
    _laboratory_handler.includeInRender(_render_handler);
    _poacher_handler.includeInRender(_render_handler);
    _render_handler.includeInRender(_player);
}