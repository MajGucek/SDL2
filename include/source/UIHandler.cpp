#include <UIHandler.h>

void UIHandler::includeInRender(RenderHandler& render_handler) {
    for (auto& x : _ui) {
        render_handler.includeInRender(x.second.get());
    }
}
bool UIHandler::handleInput(const std::string message) {
    _message = message;
    return !_close_game;
}
void StartMenu::init() {
    auto size = RenderHandler::getSize();
    int w = size.first;
    int h = size.second;
    std::shared_ptr<GameObject> start = EntityFactory::createGameObject(
        TextureType::start, {w / 2 - 400, h / 2 - 400, 800, 200});
    _ui.insert({"start", std::move(start)});

    std::shared_ptr<GameObject> load = EntityFactory::createGameObject(
        TextureType::load, {w / 2 - 400, h / 2 - 200, 800, 200});
    _ui.insert({"load", std::move(load)});

    std::shared_ptr<GameObject> settings = EntityFactory::createGameObject(
        TextureType::settings, {w / 2 - 500, h / 2, 1000, 300});
    _ui.insert({"settings", std::move(settings)});

    std::shared_ptr<GameObject> exit = EntityFactory::createGameObject(
        TextureType::exit, {w / 2 - 400, h / 2 + 400, 800, 200});
    _ui.insert({"exit", std::move(exit)});
}
std::string StartMenu::handleMenu(RenderHandler& render_handler) {
    _ui.at("start")->setTexture(TextureType::start);
    _ui.at("load")->setTexture(TextureType::load);
    _ui.at("settings")->setTexture(TextureType::settings);
    _ui.at("exit")->setTexture(TextureType::exit);
    if (_message.find("e") != std::string::npos) {
        // Enter pressed
        if (_state == StartStates::Start) {
            AudioHandler::getInstance().stopSFX();
            AudioHandler::getInstance().playSFX("twinkle");
            _finished = true;
            return "start";
        } else if (_state == StartStates::Load) {
            return "load";
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
    } else if (_state == StartStates::Load) {
        _ui.at("load")->setTexture(TextureType::load_hovered);
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

void DeathMenu::init() {
    auto size = RenderHandler::getSize();
    int w = size.first;
    int h = size.second;
    std::shared_ptr<GameObject> restart = EntityFactory::createGameObject(
        TextureType::restart, {w / 2 - 400, h / 2 - 200, 800, 200});
    _ui.insert({"restart", std::move(restart)});

    std::shared_ptr<GameObject> replay = EntityFactory::createGameObject(
        TextureType::replay, {w / 2 - 400, h / 2, 800, 200});
    _ui.insert({"replay", std::move(replay)});

    std::shared_ptr<GameObject> exit = EntityFactory::createGameObject(
        TextureType::exit, {w / 2 - 400, h / 2 + 400, 800, 200});
    _ui.insert({"exit", std::move(exit)});
}

void DeathMenu::playDeathAnimation(RenderHandler& render_handler) {
    auto size = RenderHandler::getSize();
    int w = size.first;
    int h = size.second;
    render_handler.clearRenderQueue();

    AudioHandler::getInstance().stopSFX();
    AudioHandler::getInstance().playSFX("you_died");
    render_handler.includeInRender(
        EntityFactory::createGameObject(TextureType::death_screen,
                                        {0, (h / 2) - 200, w, 300}),
        _death_animation_lenght);
    _death_animation_timer.startTimer(_death_animation_lenght);
    while (_death_animation_timer.exists()) {
        // just waiting, ik stupid
        render_handler.render();
    }
    render_handler.clearAnimationQueue();
}

void SettingsMenu::init() {
    auto size = RenderHandler::getSize();
    int w = size.first;
    int h = size.second;
    std::shared_ptr<GameObject> resolution = EntityFactory::createGameObject(
        TextureType::restart, {w / 2 - 400, h / 2 - 200, 800, 200});
    _ui.insert({"resolution", std::move(resolution)});

    std::shared_ptr<GameObject> exit = EntityFactory::createGameObject(
        TextureType::exit, {w / 2 - 400, h / 2 + 400, 800, 200});
    _ui.insert({"exit", std::move(exit)});
}

std::string SettingsMenu::handleMenu(RenderHandler& render_handler) {
    switch (_resolution) {
        case Resolutions::p2560x1440:
            _ui.at("resolution")->setTexture(TextureType::p2560x1440);
            break;
        case Resolutions::p1920x1080:
            _ui.at("resolution")->setTexture(TextureType::p1920x1080);
            break;
        case Resolutions::p1280x720:
            _ui.at("resolution")->setTexture(TextureType::p1280x720);
            break;
    }
    _ui.at("exit")->setTexture(TextureType::exit);
    if (_message.find("e") != std::string::npos) {
        // Enter pressed
        if (_state == SettingsState::Exit) {
            // close menu, return to main menu
            if (_resolution == Resolutions::p1280x720) {
                render_handler.setRenderingSize(1280, 720);
            } else if (_resolution == Resolutions::p1920x1080) {
                render_handler.setRenderingSize(1920, 1080);
            } else {
                render_handler.setRenderingSize(2560, 1440);
            }
            return "exit";
        }
    }
    if (!_navigation_timer.exists()) {
        if (_message.find("↑") != std::string::npos) {
            if (_state != SettingsState::Resolution) {
                _state = static_cast<SettingsState>(_state - 1);
            } else {
                _state = SettingsState::Exit;
            }
        } else if (_message.find("↓") != std::string::npos) {
            if (_state != SettingsState::Exit) {
                _state = static_cast<SettingsState>(_state + 1);
            } else {
                _state = SettingsState::Resolution;
            }
        }
        if (_state == SettingsState::Resolution) {
            // handle increasing, decreasing resolution
            if (_message.find("→") != std::string::npos) {
                // increase resolution
                if (_resolution != Resolutions::p2560x1440) {
                    _resolution = static_cast<Resolutions>(_resolution + 1);
                } else {
                    // smo na 1440p in povecamo
                    _resolution = Resolutions::p1280x720;
                }
            } else if (_message.find("←") != std::string::npos) {
                // decrease resolution
                if (_resolution != Resolutions::p1280x720) {
                    _resolution = static_cast<Resolutions>(_resolution - 1);
                } else {
                    _resolution = Resolutions::p2560x1440;
                }
            }
        }

        _navigation_timer.startTimer(_input_delay);
    }
    if (_state == SettingsState::Resolution) {
        switch (_resolution) {
            case Resolutions::p2560x1440:
                _ui.at("resolution")
                    ->setTexture(TextureType::p2560x1440_hovered);
                break;
            case Resolutions::p1920x1080:
                _ui.at("resolution")
                    ->setTexture(TextureType::p1920x1080_hovered);
                break;
            case Resolutions::p1280x720:
                _ui.at("resolution")
                    ->setTexture(TextureType::p1280x720_hovered);
                break;
        }

    } else if (_state == SettingsState::Exit) {
        _ui.at("exit")->setTexture(TextureType::exit_hovered);
    }
    _message = "";
    return "";
}

void PauseMenu::init() {
    auto size = RenderHandler::getSize();
    int w = size.first;
    int h = size.second;
    auto play = EntityFactory::createGameObject(
        TextureType::play, {w / 2 - 400, h / 2 - 400, 800, 200});
    _ui.insert({"play", std::move(play)});

    std::shared_ptr<GameObject> save = EntityFactory::createGameObject(
        TextureType::save, {w / 2 - 400, h / 2, 800, 200});
    _ui.insert({"save", std::move(save)});
    std::shared_ptr<GameObject> exit = EntityFactory::createGameObject(
        TextureType::exit, {w / 2 - 400, h / 2 + 400, 800, 200});
    _ui.insert({"exit", std::move(exit)});
}

std::string PauseMenu::handleMenu(RenderHandler& render_handler) {
    _ui.at("play")->setTexture(TextureType::play);
    _ui.at("save")->setTexture(TextureType::save);
    _ui.at("exit")->setTexture(TextureType::exit);
    if (_message.find("e") != std::string::npos) {
        // Enter pressed
        if (_state == PauseState::Play) {
            _finished = true;
            return "play";
        } else if (_state == PauseState::Save) {
            // open settings menu
            return "save";
        } else if (_state == PauseState::Exit) {
            // terminate program
            _close_game = true;
            return "exit";
        }
    }
    if (!_navigation_timer.exists()) {
        if (_message.find("↑") != std::string::npos) {
            if (_state != PauseState::Play) {
                _state = static_cast<PauseState>(_state - 1);
            } else {
                _state = PauseState::Exit;
            }
        } else if (_message.find("↓") != std::string::npos) {
            if (_state != PauseState::Exit) {
                _state = static_cast<PauseState>(_state + 1);
            } else {
                _state = PauseState::Play;
            }
        }
        _navigation_timer.startTimer(_input_delay);
    }

    if (_state == PauseState::Play) {
        _ui.at("play")->setTexture(TextureType::play_hovered);
    } else if (_state == PauseState::Save) {
        _ui.at("save")->setTexture(TextureType::save_hovered);
    } else if (_state == PauseState::Exit) {
        _ui.at("exit")->setTexture(TextureType::exit_hovered);
    }
    _message = "";
    return "";
}

void LoginMenu::init() {
    auto size = RenderHandler::getSize();
    int w = size.first;
    int h = size.second;

    std::shared_ptr<GameObject> input = EntityFactory::createGameObject(
        TextureType::input, {w / 2 - 1000, h / 2 - 400, 2000, 100});
    _ui.insert({"input", std::move(input)});

    std::shared_ptr<GameObject> confirm = EntityFactory::createGameObject(
        TextureType::confirm, {w / 2 - 400, h / 2, 800, 200});
    _ui.insert({"confirm", std::move(confirm)});
    std::shared_ptr<GameObject> exit = EntityFactory::createGameObject(
        TextureType::exit, {w / 2 - 400, h / 2 + 400, 800, 200});
    _ui.insert({"exit", std::move(exit)});
}

std::string LoginMenu::handleMenu(RenderHandler& render_handler) {
    _ui.at("input")->setTexture(TextureType::input);
    _ui.at("confirm")->setTexture(TextureType::confirm);
    _ui.at("exit")->setTexture(TextureType::exit);

    if (_message.find("e") != std::string::npos) {
        // Enter pressed
        if (_state == LoginState::Input && !_enter_timer.exists()) {
            is_inputing = !is_inputing;
            _enter_timer.startTimer(_input_delay);
        } else if (_state == LoginState::Confirm) {
            return "confirm";
        } else if (_state == LoginState::Exit) {
            _close_game = true;
            return "exit";
        }
    }

    if (!_char_input_timer.exists() && is_inputing && _message.size() != 0) {
        char input_letter = _message.at(0);
        if (input_letter >= 'A' and input_letter <= 'Z') {
            if (_name.size() < _name_size) {
                _name.push_back(input_letter);
            }
        } else if (input_letter == 's') {
            if (_name.size() < _name_size) {
                _name.push_back(' ');
            }
        } else if (input_letter == 'b') {
            if (_name.size() > 0) {
                _name.pop_back();
            }
        }
        _char_input_timer.startTimer(_char_input_delay);
    }

    if (!_navigation_timer.exists() && !is_inputing) {
        if (_message.find("↑") != std::string::npos) {
            if (_state != LoginState::Input) {
                _state = static_cast<LoginState>(_state - 1);
            } else {
                _state = LoginState::Exit;
            }
        } else if (_message.find("↓") != std::string::npos) {
            if (_state != LoginState::Exit) {
                _state = static_cast<LoginState>(_state + 1);
            } else {
                _state = LoginState::Input;
            }
        }
        _navigation_timer.startTimer(_input_delay);
    }
    if (_state == LoginState::Input) {
        if (is_inputing) {
            _ui.at("input")->setTexture(TextureType::input_inputing);
        } else {
            _ui.at("input")->setTexture(TextureType::input_hovered);
        }
    } else if (_state == LoginState::Confirm) {
        _ui.at("confirm")->setTexture(TextureType::confirm_hovered);
    } else if (_state == LoginState::Exit) {
        _ui.at("exit")->setTexture(TextureType::exit_hovered);
    }

    _message = "";
    return "";
}

void LoginMenu::includeInRender(RenderHandler& render_handler) {
    UIHandler::includeInRender(render_handler);
    auto size = RenderHandler::getSize();
    int w = size.first;
    int h = size.second;
    // h / 2 - 400
    // w / 2 - 1000
    for (int i = 0; i < _name.size(); ++i) {
        // std::cout << "inc" << std::endl;
        char letter = _name.at(i);
        SDL_Rect hb = {(w / 2 - 1000) + i * 100, h / 2 - 400, 100, 100};
        TextureType tex;
        switch (letter) {
            case 'A':
                tex = TextureType::A;
                break;
            case 'B':
                tex = TextureType::B;
                break;
            case 'C':
                tex = TextureType::C;
                break;
            case 'D':
                tex = TextureType::D;
                break;
            case 'E':
                tex = TextureType::E;
                break;
            case 'F':
                tex = TextureType::F;
                break;
            case 'G':
                tex = TextureType::G;
                break;
            case 'H':
                tex = TextureType::H;
                break;
            case 'I':
                tex = TextureType::I;
                break;
            case 'J':
                tex = TextureType::J;
                break;
            case 'K':
                tex = TextureType::K;
                break;
            case 'L':
                tex = TextureType::L;
                break;
            case 'M':
                tex = TextureType::M;
                break;
            case 'N':
                tex = TextureType::N;
                break;
            case 'O':
                tex = TextureType::O;
                break;
            case 'P':
                tex = TextureType::P;
                break;
            case 'Q':
                tex = TextureType::Q;
                break;
            case 'R':
                tex = TextureType::R;
                break;
            case 'S':
                tex = TextureType::S;
                break;
            case 'T':
                tex = TextureType::T;
                break;
            case 'U':
                tex = TextureType::U;
                break;
            case 'V':
                tex = TextureType::V;
                break;
            case 'W':
                tex = TextureType::W;
                break;
            case 'X':
                tex = TextureType::X;
                break;
            case 'Y':
                tex = TextureType::Y;
                break;
            case 'Z':
                tex = TextureType::Z;
                break;
            default:
                tex = TextureType::Unknown;
                break;
        }
        auto let = EntityFactory::createGameObject(tex, hb);
        render_handler.includeInRender(std::move(let), 1);
    }
}

char* LoginMenu::getName() {
    char* name = new char[_name_size];
    strncpy(name, _name.c_str(), _name_size);
    return name;
}

std::shared_ptr<StartMenu> UIFactory::createStartMenu() {
    return std::make_shared<StartMenu>();
}

std::shared_ptr<DeathMenu> UIFactory::createDeathMenu() {
    return std::make_shared<DeathMenu>();
}

std::shared_ptr<SettingsMenu> UIFactory::createSettingsMenu() {
    return std::make_shared<SettingsMenu>();
}

std::shared_ptr<PauseMenu> UIFactory::createPauseMenu() {
    return std::make_shared<PauseMenu>();
}

std::shared_ptr<LoginMenu> UIFactory::createLoginMenu() {
    return std::make_shared<LoginMenu>();
}
