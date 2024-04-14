#include <headers/RenderHandler.h>

int RenderHandler::_r_w = 3200;
int RenderHandler::_r_h = 1800;

RenderHandler::RenderHandler()
    : _window(nullptr),
      _renderer(nullptr),
      _w(1920),
      _h(1080),
      _background({0, 0, _w, _h}) {}
RenderHandler::~RenderHandler() {
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    _window = nullptr;
    _renderer = nullptr;
}
RenderHandler &RenderHandler::initSystem() {
    SDL_Init(SDL_INIT_EVERYTHING);
    Mix_Init(MIX_INIT_MP3);
    IMG_Init(IMG_INIT_PNG);
    return *this;
}
RenderHandler &RenderHandler::createWindow(int screen_width,
                                           int screen_height) {
    _window = SDL_CreateWindow("", 0, 0, screen_width, screen_height,
                               SDL_WINDOW_MOUSE_CAPTURE);
    setRenderingSize(screen_width, screen_height);
    return *this;
}
RenderHandler &RenderHandler::createRenderer(int index, Uint32 flags) {
    if (!_window) {
        std::cout << "No window, failed to create Renderer!" << std::endl;
        return *this;
    }
    _renderer = SDL_CreateRenderer(_window, index, flags);
    return *this;
}
SDL_Renderer *RenderHandler::getRenderer() { return _renderer; }

void RenderHandler::includeInRender(GameObject *entity) {
    _entities.push_back(entity);
}

void RenderHandler::includeInRender(std::unique_ptr<GameObject> entity,
                                    int frames) {
    _animation_entities.push_back({std::move(entity), frames});
}

void RenderHandler::render() {
    float scale_factor = static_cast<float>(_h) / static_cast<float>(_r_h);
    // clear screen
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);
    SDL_RenderCopy(
        _renderer,
        TextureHandler::getInstance().getTexture(TextureType::background),
        nullptr, &_background);

    for (auto &entity : _entities) {
        auto hb = *entity->getHitbox();
        hb.x *= scale_factor;
        hb.y *= scale_factor;
        hb.w *= scale_factor;
        hb.h *= scale_factor;
        SDL_RenderCopy(_renderer, entity->getTexture(), nullptr, &hb);
    }

    for (auto it = _animation_entities.begin(); it != _animation_entities.end();
         it++) {
        if ((*it).second > 0) {
            auto hb = *(*it).first->getHitbox();
            hb.x *= scale_factor;
            hb.y *= scale_factor;
            hb.w *= scale_factor;
            hb.h *= scale_factor;
            SDL_RenderCopy(_renderer, (*it).first->getTexture(), nullptr, &hb);
            (*it).second--;
        } else {
            _animation_entities.erase(it);
        }
    }

    // render scoreboard
    int score = Scoreboard::getInstance().getScore();
    std::vector<int> nums;
    while (score > 0) {
        nums.push_back(score % 10);
        score /= 10;
    }
    std::reverse(nums.begin(), nums.end());
    // ++++ //
    for (int i = 0; i < nums.size(); ++i) {
        SDL_Texture *digit =
            Scoreboard::getInstance().getDigitTexture(nums.at(i), _renderer);
        SDL_Rect score_hitbox = {i * 100, 0, 100, 100};
        SDL_RenderCopy(_renderer, digit, nullptr, &score_hitbox);
        SDL_DestroyTexture(digit);
    }

    // ++++ //
    SDL_RenderPresent(_renderer);
    _entities.clear();
}

void RenderHandler::clearRenderQueue() { _entities.clear(); }
void RenderHandler::clearAnimationQueue() { _animation_entities.clear(); }

void RenderHandler::setRenderingSize(int w, int h) {
    SDL_SetWindowSize(_window, w, h);
    SDL_SetWindowPosition(_window, 0, 0);
    _w = w;
    _h = h;
    _background.w = _w;
    _background.h = _h;
}

std::pair<int, int> RenderHandler::getRenderingSize() { return {_w, _h}; }
std::pair<int, int> RenderHandler::getSize() { return {_r_w, _r_h}; }

void TextureHandler::init(SDL_Renderer *renderer) {
    _renderer = renderer;
    loadTextures();
}

TextureHandler::TextureHandler() : _renderer(nullptr) {}

TextureHandler &TextureHandler::getInstance() {
    static TextureHandler t;
    return t;
}

void TextureHandler::loadTextures() {
    if (!_renderer) {
    }
    // GAMEPLAY //
    _textures.insert({TextureType::background,
                      IMG_LoadTexture(_renderer, "res/background.png")});
    _textures.insert(
        {TextureType::player, IMG_LoadTexture(_renderer, "res/player.png")});
    _textures.insert({TextureType::player_hit,
                      IMG_LoadTexture(_renderer, "res/player_hit.png")});
    _textures.insert(
        {TextureType::hit, IMG_LoadTexture(_renderer, "res/hit.png")});
    _textures.insert(
        {TextureType::hint, IMG_LoadTexture(_renderer, "res/hint.png")});
    _textures.insert({TextureType::laboratory,
                      IMG_LoadTexture(_renderer, "res/laboratory.png")});
    _textures.insert({TextureType::laboratory_hit,
                      IMG_LoadTexture(_renderer, "res/laboratory_hit.png")});
    _textures.insert(
        {TextureType::poacher, IMG_LoadTexture(_renderer, "res/poacher.png")});
    _textures.insert({TextureType::poacher_hit,
                      IMG_LoadTexture(_renderer, "res/poacher_hit.png")});
    _textures.insert({TextureType::pangolin,
                      IMG_LoadTexture(_renderer, "res/pangolin_inactive.png")});
    _textures.insert({TextureType::pangolin_active,
                      IMG_LoadTexture(_renderer, "res/pangolin_active.png")});
    _textures.insert({TextureType::ground_rune,
                      IMG_LoadTexture(_renderer, "res/ground_rune.png")});
    // SCORE //
    _textures.insert(
        {TextureType::score_0, IMG_LoadTexture(_renderer, "res/score/0.png")});
    _textures.insert(
        {TextureType::score_1, IMG_LoadTexture(_renderer, "res/score/1.png")});
    _textures.insert(
        {TextureType::score_2, IMG_LoadTexture(_renderer, "res/score/2.png")});
    _textures.insert(
        {TextureType::score_3, IMG_LoadTexture(_renderer, "res/score/3.png")});
    _textures.insert(
        {TextureType::score_4, IMG_LoadTexture(_renderer, "res/score/4.png")});
    _textures.insert(
        {TextureType::score_5, IMG_LoadTexture(_renderer, "res/score/5.png")});
    _textures.insert(
        {TextureType::score_6, IMG_LoadTexture(_renderer, "res/score/6.png")});
    _textures.insert(
        {TextureType::score_7, IMG_LoadTexture(_renderer, "res/score/7.png")});
    _textures.insert(
        {TextureType::score_8, IMG_LoadTexture(_renderer, "res/score/8.png")});
    _textures.insert(
        {TextureType::score_9, IMG_LoadTexture(_renderer, "res/score/9.png")});

    _textures.insert(
        {TextureType::A, IMG_LoadTexture(_renderer, "res/letters/A.png")});
    _textures.insert(
        {TextureType::B, IMG_LoadTexture(_renderer, "res/letters/B.png")});
    _textures.insert(
        {TextureType::C, IMG_LoadTexture(_renderer, "res/letters/C.png")});
    _textures.insert(
        {TextureType::D, IMG_LoadTexture(_renderer, "res/letters/D.png")});
    _textures.insert(
        {TextureType::E, IMG_LoadTexture(_renderer, "res/letters/E.png")});
    _textures.insert(
        {TextureType::F, IMG_LoadTexture(_renderer, "res/letters/F.png")});
    _textures.insert(
        {TextureType::G, IMG_LoadTexture(_renderer, "res/letters/G.png")});
    _textures.insert(
        {TextureType::H, IMG_LoadTexture(_renderer, "res/letters/H.png")});
    _textures.insert(
        {TextureType::I, IMG_LoadTexture(_renderer, "res/letters/I.png")});
    _textures.insert(
        {TextureType::J, IMG_LoadTexture(_renderer, "res/letters/J.png")});
    _textures.insert(
        {TextureType::K, IMG_LoadTexture(_renderer, "res/letters/K.png")});
    _textures.insert(
        {TextureType::L, IMG_LoadTexture(_renderer, "res/letters/L.png")});
    _textures.insert(
        {TextureType::M, IMG_LoadTexture(_renderer, "res/letters/M.png")});
    _textures.insert(
        {TextureType::N, IMG_LoadTexture(_renderer, "res/letters/N.png")});
    _textures.insert(
        {TextureType::O, IMG_LoadTexture(_renderer, "res/letters/O.png")});
    _textures.insert(
        {TextureType::P, IMG_LoadTexture(_renderer, "res/letters/P.png")});
    _textures.insert(
        {TextureType::Q, IMG_LoadTexture(_renderer, "res/letters/Q.png")});
    _textures.insert(
        {TextureType::R, IMG_LoadTexture(_renderer, "res/letters/R.png")});
    _textures.insert(
        {TextureType::S, IMG_LoadTexture(_renderer, "res/letters/S.png")});
    _textures.insert(
        {TextureType::T, IMG_LoadTexture(_renderer, "res/letters/T.png")});
    _textures.insert(
        {TextureType::U, IMG_LoadTexture(_renderer, "res/letters/U.png")});
    _textures.insert(
        {TextureType::V, IMG_LoadTexture(_renderer, "res/letters/V.png")});
    _textures.insert(
        {TextureType::W, IMG_LoadTexture(_renderer, "res/letters/W.png")});
    _textures.insert(
        {TextureType::X, IMG_LoadTexture(_renderer, "res/letters/X.png")});
    _textures.insert(
        {TextureType::Y, IMG_LoadTexture(_renderer, "res/letters/Y.png")});
    _textures.insert(
        {TextureType::Z, IMG_LoadTexture(_renderer, "res/letters/Z.png")});
    _textures.insert({TextureType::Unknown,
                      IMG_LoadTexture(_renderer, "res/letters/Unknown.png")});
    // UI //
    _textures.insert(
        {TextureType::start, IMG_LoadTexture(_renderer, "res/ui/start.png")});
    _textures.insert({TextureType::start_hovered,
                      IMG_LoadTexture(_renderer, "res/ui/start_hovered.png")});
    _textures.insert({TextureType::settings,
                      IMG_LoadTexture(_renderer, "res/ui/settings.png")});
    _textures.insert(
        {TextureType::settings_hovered,
         IMG_LoadTexture(_renderer, "res/ui/settings_hovered.png")});
    _textures.insert(
        {TextureType::exit, IMG_LoadTexture(_renderer, "res/ui/exit.png")});
    _textures.insert({TextureType::exit_hovered,
                      IMG_LoadTexture(_renderer, "res/ui/exit_hovered.png")});
    _textures.insert({TextureType::restart,
                      IMG_LoadTexture(_renderer, "res/ui/restart.png")});
    _textures.insert(
        {TextureType::restart_hovered,
         IMG_LoadTexture(_renderer, "res/ui/restart_hovered.png")});
    _textures.insert(
        {TextureType::play, IMG_LoadTexture(_renderer, "res/ui/play.png")});
    _textures.insert({TextureType::play_hovered,
                      IMG_LoadTexture(_renderer, "res/ui/play_hovered.png")});
    _textures.insert(
        {TextureType::replay, IMG_LoadTexture(_renderer, "res/ui/replay.png")});
    _textures.insert({TextureType::replay_hovered,
                      IMG_LoadTexture(_renderer, "res/ui/replay_hovered.png")});
    _textures.insert(
        {TextureType::save, IMG_LoadTexture(_renderer, "res/ui/save.png")});
    _textures.insert({TextureType::save_hovered,
                      IMG_LoadTexture(_renderer, "res/ui/save_hovered.png")});
    _textures.insert(
        {TextureType::load, IMG_LoadTexture(_renderer, "res/ui/load.png")});
    _textures.insert({TextureType::load_hovered,
                      IMG_LoadTexture(_renderer, "res/ui/load_hovered.png")});
    _textures.insert({TextureType::leaderboard,
                      IMG_LoadTexture(_renderer, "res/ui/leaderboard.png")});
    _textures.insert(
        {TextureType::leaderboard_hovered,
         IMG_LoadTexture(_renderer, "res/ui/leaderboard_hovered.png")});

    _textures.insert(
        {TextureType::input, IMG_LoadTexture(_renderer, "res/ui/input.png")});
    _textures.insert({TextureType::input_hovered,
                      IMG_LoadTexture(_renderer, "res/ui/input_hovered.png")});
    _textures.insert({TextureType::input_inputing,
                      IMG_LoadTexture(_renderer, "res/ui/input_inputing.png")});
    _textures.insert({TextureType::confirm,
                      IMG_LoadTexture(_renderer, "res/ui/confirm.png")});
    _textures.insert(
        {TextureType::confirm_hovered,
         IMG_LoadTexture(_renderer, "res/ui/confirm_hovered.png")});
    _textures.insert(
        {TextureType::empty, IMG_LoadTexture(_renderer, "res/ui/empty.png")});

    _textures.insert({TextureType::p2560x1440,
                      IMG_LoadTexture(_renderer, "res/ui/p2560x1440.png")});
    _textures.insert(
        {TextureType::p2560x1440_hovered,
         IMG_LoadTexture(_renderer, "res/ui/p2560x1440_hovered.png")});
    _textures.insert({TextureType::p1920x1080,
                      IMG_LoadTexture(_renderer, "res/ui/p1920x1080.png")});
    _textures.insert(
        {TextureType::p1920x1080_hovered,
         IMG_LoadTexture(_renderer, "res/ui/p1920x1080_hovered.png")});
    _textures.insert({TextureType::p1280x720,
                      IMG_LoadTexture(_renderer, "res/ui/p1280x720.png")});
    _textures.insert(
        {TextureType::p1280x720_hovered,
         IMG_LoadTexture(_renderer, "res/ui/p1280x720_hovered.png")});

    _textures.insert({TextureType::death_screen,
                      IMG_LoadTexture(_renderer, "res/ui/death_screen.png")});
}

SDL_Texture *TextureHandler::getTexture(TextureType texture_type) {
    if (_renderer) {
        auto pair = _textures.find(texture_type);
        if (pair != _textures.end()) {
            return pair->second;
        } else {
            std::cout << "invalid texture type, this should never happen!"
                      << std::endl;
            return nullptr;
        }
    } else {
        std::cout << "no renderer given!" << std::endl;
        return nullptr;
    }
}

TextureHandler::~TextureHandler() {
    for (auto &x : _textures) {
        SDL_DestroyTexture(x.second);
    }
}