#include <headers/RenderHandler.h>

RenderHandler::RenderHandler() {
    _window = nullptr;
    _renderer = nullptr;
    _scoreboard = nullptr;
}
RenderHandler::~RenderHandler() {
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    _window = nullptr;
    _renderer = nullptr;
    _scoreboard = nullptr;
}
RenderHandler &RenderHandler::initSystem() {
    SDL_Init(SDL_INIT_EVERYTHING);
    return *this;
}
RenderHandler &RenderHandler::createWindow(int screen_width,
                                           int screen_height) {
    _window =
        SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                         screen_width, screen_height, SDL_WINDOW_SHOWN);
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

void RenderHandler::includeInRender(std::shared_ptr<GameObject> entity) {
    _entities.push_back(std::move(entity));
}
void RenderHandler::includeInRender(std::shared_ptr<GameObject> entity,
                                    int frames) {
    _animation_entities.push_back({std::move(entity), frames});
}

void RenderHandler::includeScoreboardInRender(Scoreboard *scoreboard) {
    _scoreboard = scoreboard;
}

void RenderHandler::render() {
    // clear screen
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);

    for (auto &entity : _entities) {
        SDL_RenderCopy(_renderer, entity->getTexture(), nullptr,
                       entity->getHitbox());
    }
    for (auto &an_entity : _animation_entities) {
        if (an_entity.second > 0) {
            SDL_RenderCopy(_renderer, an_entity.first->getTexture(), nullptr,
                           an_entity.first->getHitbox());
            an_entity.second--;
        }
    }

    // render scoreboard
    if (_scoreboard) {
        int score = _scoreboard->getScore();
        std::vector<int> nums;
        while (score > 0) {
            nums.push_back(score % 10);
            score /= 10;
        }
        std::reverse(nums.begin(), nums.end());
        // ++++ //
        for (int i = 0; i < nums.size(); ++i) {
            SDL_Texture *digit =
                _scoreboard->getDigitTexture(nums.at(i), _renderer);
            SDL_Rect score_hitbox = {i * 100, 0, 100, 100};
            SDL_RenderCopy(_renderer, digit, nullptr, &score_hitbox);
            // SDL_DestroyTexture(digit);
        }
    }

    // ++++ //
    SDL_RenderPresent(_renderer);
    _entities.clear();
}