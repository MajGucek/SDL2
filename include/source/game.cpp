#include <headers/game.h>

Game::Game()
    : _screen_width(2560),
      _screen_height(1440),
      _game_state(GameState::PLAY),
      _fps(144) {
    TimeHandler::getInstance().setFramerate(_fps);
    _render_handler.initSystem()
        .createWindow(_screen_width, _screen_height)
        .createRenderer();
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
void Game::stop() { _game_state = GameState::EXIT; }

void Game::init() {
    // create background
    _background = EntityFactory::createGameObject(
        TextureType::background, _render_handler.getRenderer(),
        {0, 0, _screen_width, _screen_height});

    _start_menu = UIFactory::createStartMenu();
    _start_menu->init(_screen_width, _screen_height,
                      _render_handler.getRenderer());
    _input_handler.subscribe(_start_menu);

    while (!_start_menu->isFinished() && _game_state == GameState::PLAY) {
        if (!_input_handler.handleInput(&_render_handler)) {
            _game_state = GameState::EXIT;
        }
        _render_handler.includeInRender(_background);
        _start_menu->includeInRender(_render_handler);
        _render_handler.render();
    }

    // create player
    _player = EntityFactory::createPlayer(_render_handler.getRenderer(),
                                          {200, 200, 100, 100}, 100, 150);
    _input_handler.subscribe(_player);
    _player->addCollisionHandler(&_collision_handler);
    // set laboratory constants
    _laboratory_handler.setVisibility(400);
    _laboratory_handler.addLaboratory(_render_handler.getRenderer(), 700, 100,
                                      &_collision_handler, 100, 700);
    _laboratory_handler.addLaboratory(_render_handler.getRenderer(), 300, 600,
                                      &_collision_handler, 70, 4340);
    _poacher_handler.addPoacher(_render_handler.getRenderer(), 900, 900,
                                &_collision_handler, 10, 20);
}

void Game::gameLoop() {
    while (_game_state != GameState::EXIT) {
        if (!_input_handler.handleInput(&_render_handler)) {
            _game_state = GameState::EXIT;
        }
        handleEnemies();
        includeInRender();
        _render_handler.render();
    }
}

void Game::handleEnemies() {
    _laboratory_handler.handle(_player, _collision_handler, &_render_handler);
    _poacher_handler.handle(_player, _collision_handler, &_render_handler);
}

void Game::includeInRender() {
    _render_handler.includeInRender(_background);
    _laboratory_handler.includeInRender(_render_handler);
    _poacher_handler.includeInRender(_render_handler);
    _render_handler.includeInRender(_player);
}
