#include <headers/game.h>

Game::Game()
    : _screen_width(1920),
      _screen_height(1080),
      _game_state(GameState::PLAY),
      _fps(60) {}
Game::~Game() { SDL_Quit(); }
float Game::getDeltaTime() { return _delta_time; }

void Game::run() {
    init();
    gameLoop();
}

void Game::init() {
    TimeHandler::getInstance().setFramerate(_fps);
    // init renderer
    _render_handler.initSystem()
        .createWindow(_screen_width, _screen_height)
        .createRenderer();

    // init collision handling
    _collision_handler.setSize(_screen_width, _screen_height);

    // create background
    _background = EntityFactory::createGameObject(
        "res/background.png", _render_handler.getRenderer(),
        {0, 0, _screen_width, _screen_height});

    // create player
    _player = EntityFactory::createPlayer("res/player.png",
                                          _render_handler.getRenderer(),
                                          {200, 200, 100, 100}, 100, 15);
    _input_handler.subscribe(_player);
    _player->addCollisionHandler(&_collision_handler);

    // set laboratory constants
    _laboratory_handler.setVisibility(400);
    _laboratory_handler.addLaboratory(_render_handler.getRenderer(), 700, 100,
                                      &_collision_handler, 100, 700);
    _laboratory_handler.addLaboratory(_render_handler.getRenderer(), 300, 600,
                                      &_collision_handler, 70, 4340);
    _poacher_handler.addPoacher(_render_handler.getRenderer(), 900, 900,
                                &_collision_handler, 10, 10);
}
void Game::handleEvents(float delta_time) {
    // player gets handled in handleInput
    if (_input_handler.handleInput(delta_time, &_render_handler,
                                   &_scoreboard) == "exit") {
        _game_state = GameState::EXIT;
    }
}

void Game::gameLoop() {
    while (_game_state != GameState::EXIT) {
        handleEvents(_delta_time);
        handleEnemies();
        includeInRender();
        _render_handler.render();
        TimeHandler::getInstance().handleFramerate();
    }
}

void Game::handleEnemies() {
    _laboratory_handler.handle(_player, _scoreboard, _collision_handler,
                               &_render_handler);
    _poacher_handler.handle(_player, _scoreboard, _collision_handler,
                            &_render_handler);
}

void Game::includeInRender() {
    _render_handler.includeInRender(_background);
    _laboratory_handler.includeInRender(_render_handler);
    _poacher_handler.includeInRender(_render_handler);
    _render_handler.includeInRender(_player);
    _render_handler.includeScoreboardInRender(&_scoreboard);
}
