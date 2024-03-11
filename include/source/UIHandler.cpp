#include <UIHandler.h>

void StartMenu::init(int w, int h, SDL_Renderer* ren) {
    std::shared_ptr<GameObject> start = EntityFactory::createGameObject(
        "res/ui/start.png", ren, {w / 2 - 400, h / 2 - 200, 800, 200});

    _ui.insert({"start", std::move(start)});
    std::shared_ptr<GameObject> settings = EntityFactory::createGameObject(
        "res/ui/settings.png", ren, {w / 2 - 500, h / 2, 1000, 300});
    _ui.insert({"settings", std::move(settings)});

    std::shared_ptr<GameObject> exit = EntityFactory::createGameObject(
        "res/ui/exit.png", ren, {w / 2 - 400, h / 2 + 400, 800, 200});
    _ui.insert({"exit", std::move(exit)});
    _state = StartStates::Start;
}

bool StartMenu::isFinished() { return _finished; }

bool StartMenu::handleInput(const std::string message,
                            RenderHandler* render_handler) {
    _ui.at("start")->setTexture("res/ui/start.png",
                                render_handler->getRenderer());
    _ui.at("settings")
        ->setTexture("res/ui/settings.png", render_handler->getRenderer());
    _ui.at("exit")->setTexture("res/ui/exit.png",
                               render_handler->getRenderer());
    if (message.find("e") != std::string::npos) {
        // Enter pressed
        if (_state == StartStates::Start) {
            _finished = true;
        } else if (_state == StartStates::Settings) {
            // open settings menu
        } else if (_state == StartStates::Exit) {
            // terminate program
            return false;
        }
    }
    if (!_navigation_timer.exists()) {
        if (message.find("↑") != std::string::npos) {
            std::cout << "up menu" << std::endl;
            if (_state != StartStates::Start) {
                _state = static_cast<StartStates>(_state - 1);
            } else {
                _state = StartStates::Exit;
            }
        } else if (message.find("↓") != std::string::npos) {
            std::cout << "down menu" << std::endl;
            if (_state != StartStates::Settings) {
                _state = static_cast<StartStates>(_state + 1);
            } else {
                _state = StartStates::Start;
            }
        }
        _navigation_timer.startTimer(_delay);
    }

    if (_state == StartStates::Start) {
        _ui.at("start")->setTexture("res/ui/start_hovered.png",
                                    render_handler->getRenderer());
    } else if (_state == StartStates::Settings) {
        _ui.at("settings")
            ->setTexture("res/ui/settings_hovered.png",
                         render_handler->getRenderer());
    } else if (_state == StartStates::Exit) {
        _ui.at("exit")->setTexture("res/ui/exit_hovered.png",
                                   render_handler->getRenderer());
    }
    return true;
}

void StartMenu::includeInRender(RenderHandler& render_handler) {
    for (auto& x : _ui) {
        render_handler.includeInRender(x.second);
    }
}

std::unique_ptr<StartMenu> UIFactory::createStartMenu() {
    auto ui = std::make_unique<StartMenu>();
    return std::move(ui);
}