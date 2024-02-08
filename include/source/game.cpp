#include <headers/game.h>

Game::Game() {
    _window = nullptr;
    _renderer = nullptr;
    _screen_width = 2560;
    _screen_height = 1440;
    _game_state = GameState::PLAY;
    _prev_time = SDL_GetTicks();
    _delta_time = 0.0f;
}

Game::~Game() {
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
    delete _player;
    delete _background;
    delete _rock;
}

float Game::getDeltaTime() { return _delta_time; }

void Game::run() {
    init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screen_width,
         _screen_height, SDL_WINDOW_SHOWN);

    gameLoop();
}

void Game::init(const char* title, int x, int y, int w, int h, Uint32 flags) {
    SDL_Init(SDL_INIT_EVERYTHING);

    _window = SDL_CreateWindow(title, x, y, w, h, flags);
    _renderer = SDL_CreateRenderer(_window, -1, 0);
    _surface = SDL_GetWindowSurface(_window);

    _background = EntityFactory::createEntity(
        "res/background.png", _renderer, {0, 0, _screen_width, _screen_height});

    _player = EntityFactory::createPlayer("res/player.png", _renderer,
                                          {200, 200, 100, 100}, 750);
    _rock = EntityFactory::createEntity("res/sigma.png", _renderer,
                                        {400, 400, 100, 100});

    _input_handler.subscribe(_player);

    _rock->isCollider(&_collision_handler);
    _player->isCollider(&_collision_handler);
}

void Game::render() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);

    SDL_RenderCopy(_renderer, _background->getTexture(), nullptr,
                   _background->getHitbox());

    SDL_RenderCopy(_renderer, _player->getTexture(), nullptr,
                   _player->getHitbox());

    SDL_RenderCopy(_renderer, _rock->getTexture(), nullptr, _rock->getHitbox());

    SDL_RenderPresent(_renderer);
}

void Game::gameLoop() {
    while (_game_state != GameState::EXIT) {
        Uint32 current_time = SDL_GetTicks();
        _delta_time = (current_time - _prev_time) / 1000.0f;
        _prev_time = current_time;
        handleEvents(_delta_time);
        render();
        SDL_Delay(2);
    }
}

void Game::handleEvents(float delta_time) {
    if (_input_handler.handleInput(delta_time) == "exit") {
        _game_state = GameState::EXIT;
    }
}
