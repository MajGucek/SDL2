#include <headers/game.h>

Game::Game() {
    _screen_width = 1000;
    _screen_height = 1000;
    _game_state = GameState::PLAY;
}

Game::~Game() { SDL_Quit(); }

float Game::getDeltaTime() { return _delta_time; }

void Game::run() {
    init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screen_width,
         _screen_height, SDL_WINDOW_SHOWN);

    gameLoop();
}

void Game::init(const char* title, int x, int y, int w, int h, Uint32 flags) {
    _render_handler.initSystem()
        .createWindow(title, x, y, w, h, flags)
        .createRenderer();

    _collision_handler.setSize(_screen_width, _screen_height);
    _background = EntityFactory::createEntity(
        "res/background.png", _render_handler.getRenderer(),
        {0, 0, _screen_width, _screen_height});

    _player = EntityFactory::createPlayer("res/player.png",
                                          _render_handler.getRenderer(),
                                          {200, 200, 100, 100}, 5);
    _laboratory_handler.setVisibility(400);
    _laboratory_handler.addLaboratory(_render_handler.getRenderer(), 700, 100,
                                      &_collision_handler);
    _laboratory_handler.addLaboratory(_render_handler.getRenderer(), 300, 600,
                                      &_collision_handler);

    _input_handler.subscribe(_player);

    _player->isCollider(&_collision_handler);
}

void Game::gameLoop() {
    int fps = 144;
    int desiredDelta = 1000 / fps;
    while (_game_state != GameState::EXIT) {
        handleEvents(_delta_time);

        _render_handler.includeInRender(_background);
        _laboratory_handler.includeInRender(_render_handler);
        _render_handler.includeInRender(_player);

        _render_handler.render();

        // framerate
        int startLoop = SDL_GetTicks();
        int delta = SDL_GetTicks() - startLoop;
        if (delta < desiredDelta) {
            SDL_Delay(desiredDelta - delta);
        }
    }
}

void Game::handleEvents(float delta_time) {
    // player gets handled
    if (_input_handler.handleInput(delta_time) == "exit") {
        _game_state = GameState::EXIT;
    }
    _laboratory_handler.handleLaboratories(*_player->getHitbox());
}
