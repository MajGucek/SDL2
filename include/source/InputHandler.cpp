#include <headers/InputHandler.h>

InputHandler::InputHandler() { _subscribers = {}; }

void InputHandler::notifySubs(std::string message, float delta_time) {
    for (auto& sub : _subscribers) {
        sub->handleInput(message, delta_time);
    }
}
void InputHandler::subscribe(InputListener* observer) {
    _subscribers.push_back(observer);
}
void InputHandler::unsubscribe(InputListener* observer) {
    _subscribers.remove(observer);
}

std::string InputHandler::handleInput(float delta_time) {
    SDL_Event e;
    SDL_PollEvent(&e);
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    std::string message = "";

    if (state[SDL_SCANCODE_ESCAPE]) {
        return "exit";
    }

    if (state[SDL_SCANCODE_A]) {
        message.append("A");
    }
    if (state[SDL_SCANCODE_D]) {
        message.append("D");
    }
    if (state[SDL_SCANCODE_W]) {
        message.append("W");
    }
    if (state[SDL_SCANCODE_S]) {
        message.append("S");
    }
    if (message != "") {
        notifySubs(message, delta_time);
    }

    switch (e.type) {
        case SDL_QUIT:
            return "exit";
            break;
    }

    return "";
}
