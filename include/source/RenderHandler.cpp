#include <headers/RenderHandler.h>

RenderHandler::RenderHandler() {
    _window = nullptr;
    _renderer = nullptr;
}
RenderHandler::~RenderHandler() {
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    _window = nullptr;
    _renderer = nullptr;
}
RenderHandler &RenderHandler::initSystem() {
    SDL_Init(SDL_INIT_EVERYTHING);
    return *this;
}
RenderHandler &RenderHandler::createWindow(const char *title, int x, int y,
                                           int w, int h, Uint32 flags) {
    _window = SDL_CreateWindow(title, x, y, w, h, flags);
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

void RenderHandler::includeInRender(std::shared_ptr<Entity> entity) {
    _entities.push_back(std::move(entity));
}

void RenderHandler::render() {
    // clear screen
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);

    for (auto entity : _entities) {
        SDL_RenderCopy(_renderer, entity->getTexture(), nullptr,
                       entity->getHitbox());
    }

    SDL_RenderPresent(_renderer);
}