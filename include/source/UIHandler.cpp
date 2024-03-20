#include <UIHandler.h>

void UIHandler::includeInRender(RenderHandler& render_handler) {
    for (auto& x : _ui) {
        render_handler.includeInRender(x.second);
    }
}
bool UIHandler::handleInput(const std::string message) {
    _message = message;
    return !_close_game;
}
void StartMenu::init(int w, int h) {
    std::shared_ptr<GameObject> start = EntityFactory::createGameObject(
        TextureType::start, {w / 2 - 400, h / 2 - 200, 800, 200});

    _ui.insert({"start", std::move(start)});
    std::shared_ptr<GameObject> settings = EntityFactory::createGameObject(
        TextureType::settings, {w / 2 - 500, h / 2, 1000, 300});
    _ui.insert({"settings", std::move(settings)});

    std::shared_ptr<GameObject> exit = EntityFactory::createGameObject(
        TextureType::exit, {w / 2 - 400, h / 2 + 400, 800, 200});
    _ui.insert({"exit", std::move(exit)});
    _state = StartStates::Start;
}
std::string StartMenu::handleMenu(RenderHandler& render_handler) {
    _ui.at("start")->setTexture(TextureType::start);
    _ui.at("settings")->setTexture(TextureType::settings);
    _ui.at("exit")->setTexture(TextureType::exit);
    if (_message.find("e") != std::string::npos) {
        // Enter pressed
        if (_state == StartStates::Start) {
            return "start";
            _finished = true;
        } else if (_state == StartStates::Settings) {
            // open settings menu
            return "settings";
        } else if (_state == StartStates::Exit) {
            // terminate program
            _close_game = true;
            return "exit";
        }
    }
    if (!_navigation_timer.exists()) {
        if (_message.find("↑") != std::string::npos) {
            if (_state != StartStates::Start) {
                _state = static_cast<StartStates>(_state - 1);
            } else {
                _state = StartStates::Exit;
            }
        } else if (_message.find("↓") != std::string::npos) {
            if (_state != StartStates::Exit) {
                _state = static_cast<StartStates>(_state + 1);
            } else {
                _state = StartStates::Start;
            }
        }
        _navigation_timer.startTimer(_input_delay);
    }

    if (_state == StartStates::Start) {
        _ui.at("start")->setTexture(TextureType::start_hovered);
    } else if (_state == StartStates::Settings) {
        _ui.at("settings")->setTexture(TextureType::settings_hovered);
    } else if (_state == StartStates::Exit) {
        _ui.at("exit")->setTexture(TextureType::exit_hovered);
    }
    _message = "";
    return "";
}

std::string DeathMenu::handleMenu(RenderHandler& render_handler) {
    _ui.at("restart")->setTexture(TextureType::restart);
    _ui.at("exit")->setTexture(TextureType::exit);
    if (_message.find("e") != std::string::npos) {
        // Enter pressed
        if (_state == DeathStates::Restart) {
            return "restart";
            _finished = true;
        } else if (_state == DeathStates::Exit) {
            // terminate program
            _close_game = true;
            return "exit";
        }
    }
    if (!_navigation_timer.exists()) {
        if (_message.find("↑") != std::string::npos) {
            if (_state != DeathStates::Restart) {
                _state = static_cast<DeathStates>(_state - 1);
            } else {
                _state = DeathStates::Exit;
            }
        } else if (_message.find("↓") != std::string::npos) {
            if (_state != DeathStates::Exit) {
                _state = static_cast<DeathStates>(_state + 1);
            } else {
                _state = DeathMenu::Restart;
            }
        }
        _navigation_timer.startTimer(_input_delay);
    }
    if (_state == DeathStates::Restart) {
        _ui.at("restart")->setTexture(TextureType::restart_hovered);
    } else if (_state == DeathStates::Exit) {
        _ui.at("exit")->setTexture(TextureType::exit_hovered);
    }
    _message = "";
    return "";
}

void DeathMenu::init(int w, int h) {
    std::shared_ptr<GameObject> restart = EntityFactory::createGameObject(
        TextureType::restart, {w / 2 - 400, h / 2 - 200, 800, 200});
    _ui.insert({"restart", std::move(restart)});

    std::shared_ptr<GameObject> exit = EntityFactory::createGameObject(
        TextureType::exit, {w / 2 - 400, h / 2 + 400, 800, 200});
    _ui.insert({"exit", std::move(exit)});
    _state = DeathStates::Restart;
}

void DeathMenu::playDeathAnimation(int w, int h,
                                   RenderHandler& render_handler) {
    render_handler.clearRenderQueue();
    AudioHandler::getInstance().stopSFX();
    AudioHandler::getInstance().playSFX("you_died");
    render_handler.includeInRender(
        EntityFactory::createGameObject(TextureType::death_screen,
                                        {0, (h / 2) - 200, w, 300}),
        _death_animation_lenght);
}

std::unique_ptr<StartMenu> UIFactory::createStartMenu() {
    return std::make_unique<StartMenu>();
}

std::unique_ptr<DeathMenu> UIFactory::createDeathMenu() {
    return std::make_unique<DeathMenu>();
}
