#include <headers/game.h>

Game::Game()
    : _screen_width(2560),
      _screen_height(1440),
      _game_state(GameState::PLAY),
      _fps(144),
      _desiredDelta(1000 / _fps) {}
Game::~Game() { SDL_Quit(); }
float Game::getDeltaTime() { return _delta_time; }

void Game::run() {
    init();
    gameLoop();
}

void Game::init() {
    // init renderer
    _render_handler.initSystem()
        .createWindow(_screen_width, _screen_height)
        .createRenderer();

    // init audio handler
    // .addAudio("", "audio/.mp3")
    _audio_handler.initAudio()
        .addAudio("boom", "audio/boom.mp3")
        .addAudio("circus_clown", "audio/circus_clown.mp3")
        .addAudio("death", "audio/death.mp3")
        .addAudio("hell_naw", "audio/hell_naw.mp3")
        .addAudio("money", "audio/money.mp3")
        .addAudio("shine", "audio/shine.mp3")
        .addAudio("startup", "audio/startup.mp3")
        .addAudio("yawn", "audio/yawn.mp3")
        .addAudio("you_died", "audio/you_died.mp3")
        .addAudio("zombie_hit", "audio/zombie_hit.mp3")
        .addAudio("usb_out", "audio/usb_out.mp3")
        .addAudio("usb", "audio/usb.mp3")
        .playAudio("usb")
        .addSong("posnia", "audio/posnia.mp3");

    // init collision handling
    _collision_handler.setSize(_screen_width, _screen_height);

    // create background
    _background = EntityFactory::createGameObject(
        "res/background.png", _render_handler.getRenderer(),
        {0, 0, _screen_width, _screen_height});
    _audio_handler.playAudio("usb_out");

    // create player
    _player = EntityFactory::createPlayer("res/player.png",
                                          _render_handler.getRenderer(),
                                          {200, 200, 100, 100}, 100, 5);
    _input_handler.subscribe(_player);
    _player->addCollisionHandler(&_collision_handler);

    // set laboratory constants
    _laboratory_handler.setVisibility(400);
    _laboratory_handler.addLaboratory(_render_handler.getRenderer(), 700, 100,
                                      &_collision_handler, 2000, 10);
    _laboratory_handler.addLaboratory(_render_handler.getRenderer(), 300, 600,
                                      &_collision_handler, 1000, 40);
}

void Game::gameLoop() {
    while (_game_state != GameState::EXIT) {
        handleEvents(_delta_time);
        includeInRender();
        _render_handler.render();
        handleFramerate();
    }
}

void Game::handleEvents(float delta_time) {
    // player gets handled in handleInput
    if (_input_handler.handleInput(delta_time, &_render_handler,
                                   &_scoreboard) == "exit") {
        _game_state = GameState::EXIT;
    }
    _laboratory_handler.handleLaboratories(*_player->getHitbox(), &_scoreboard);
}

void Game::includeInRender() {
    _render_handler.includeInRender(_background);
    _laboratory_handler.includeInRender(_render_handler);
    _render_handler.includeInRender(_player);
    _render_handler.includeScoreboardInRender(&_scoreboard);
}

void Game::handleFramerate() {
    int startLoop = SDL_GetTicks();
    int delta = SDL_GetTicks() - startLoop;
    if (delta < _desiredDelta) {
        SDL_Delay(_desiredDelta - delta);
    }
}