#include <UIHandler.h>

TextureType getTextureType(char x) {
    switch (x) {
        case 'A':
            return TextureType::A;
            break;
        case 'B':
            return TextureType::B;
            break;
        case 'C':
            return TextureType::C;
            break;
        case 'D':
            return TextureType::D;
            break;
        case 'E':
            return TextureType::E;
            break;
        case 'F':
            return TextureType::F;
            break;
        case 'G':
            return TextureType::G;
            break;
        case 'H':
            return TextureType::H;
            break;
        case 'I':
            return TextureType::I;
            break;
        case 'J':
            return TextureType::J;
            break;
        case 'K':
            return TextureType::K;
            break;
        case 'L':
            return TextureType::L;
            break;
        case 'M':
            return TextureType::M;
            break;
        case 'N':
            return TextureType::N;
            break;
        case 'O':
            return TextureType::O;
            break;
        case 'P':
            return TextureType::P;
            break;
        case 'Q':
            return TextureType::Q;
            break;
        case 'R':
            return TextureType::R;
            break;
        case 'S':
            return TextureType::S;
            break;
        case 'T':
            return TextureType::T;
            break;
        case 'U':
            return TextureType::U;
            break;
        case 'V':
            return TextureType::V;
            break;
        case 'W':
            return TextureType::W;
            break;
        case 'X':
            return TextureType::X;
            break;
        case 'Y':
            return TextureType::Y;
            break;
        case 'Z':
            return TextureType::Z;
            break;
        default:
            return TextureType::Unknown;
            break;
    }
}
TextureType getTextureType(int x) {
    switch (x) {
        case 0:
            return TextureType::score_0;
            break;
        case 1:
            return TextureType::score_1;
            break;
        case 2:
            return TextureType::score_2;
            break;
        case 3:
            return TextureType::score_3;
            break;
        case 4:
            return TextureType::score_4;
            break;
        case 5:
            return TextureType::score_5;
            break;
        case 6:
            return TextureType::score_6;
            break;
        case 7:
            return TextureType::score_7;
            break;
        case 8:
            return TextureType::score_8;
            break;
        case 9:
            return TextureType::score_9;
            break;
        default:
            return TextureType::Unknown;
    }
}

void Menu::includeInRender(RenderHandler& render_handler) {
    for (auto& x : _ui) {
        render_handler.includeInRender(x.second.get());
    }
}
bool Menu::handleInput(const std::string message) {
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

    std::shared_ptr<GameObject> leaderboard = EntityFactory::createGameObject(
        TextureType::leaderboard, {w / 2 - 200, h / 2, 400, 400});
    _ui.insert({"leaderboard", std::move(leaderboard)});

    std::shared_ptr<GameObject> settings = EntityFactory::createGameObject(
        TextureType::settings, {w / 2 - 500, h / 2 + 400, 1000, 300});
    _ui.insert({"settings", std::move(settings)});

    std::shared_ptr<GameObject> exit = EntityFactory::createGameObject(
        TextureType::exit, {w / 2 - 400, h / 2 + 700, 800, 200});
    _ui.insert({"exit", std::move(exit)});
}
StartMenu::StartMenu() : _hp(-1), _level(-1) {}

std::string StartMenu::handleMenu(RenderHandler& render_handler) {
    bool load = false;
    _ui.at("start")->setTexture(TextureType::start);
    _ui.at("load")->setTexture(TextureType::load);
    _ui.at("leaderboard")->setTexture(TextureType::leaderboard);
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
            load = true;
        } else if (_state == StartStates::Leaderboard) {
            return "leaderboard";
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
    } else if (_state == StartStates::Leaderboard) {
        _ui.at("leaderboard")->setTexture(TextureType::leaderboard_hovered);
    } else if (_state == StartStates::Settings) {
        _ui.at("settings")->setTexture(TextureType::settings_hovered);
    } else if (_state == StartStates::Exit) {
        _ui.at("exit")->setTexture(TextureType::exit_hovered);
    }
    _message = "";
    if (load) {
        return "load";
    }
    return "";
}

void StartMenu::setPlayerDisplay(std::string player_name, int hp, int level) {
    _nametag = player_name;
    _hp = hp;
    _level = level;
}

void StartMenu::includeInRender(RenderHandler& render_handler) {
    Menu::includeInRender(render_handler);
    auto size = RenderHandler::getSize();
    int w = size.first;
    int h = size.second;
    // h / 2 - 600
    // w / 2 - 1000
    int offset = (_nametag.size() / 2.0) * 100;
    for (int i = 0; i < _nametag.size(); ++i) {
        char letter = _nametag.at(i);
        SDL_Rect hb = {(w / 2 - offset) + (i * 100), h / 2 - 800, 100, 100};
        TextureType tex;
        tex = getTextureType(letter);
        auto let = EntityFactory::createGameObject(tex, hb);
        render_handler.includeInRender(std::move(let), 1);
    }
    if (_hp > -1 and _level > -1) {
        std::vector<int> hp_digits;
        int hp = _hp;
        while (hp > 0) {
            hp_digits.push_back(hp % 10);
            hp /= 10;
        }
        std::reverse(hp_digits.begin(), hp_digits.end());
        if (hp_digits.size() == 0) {
            hp_digits.push_back(0);
        }
        offset = (hp_digits.size() / 2.0) * 100;
        for (int i = 0; i < hp_digits.size(); ++i) {
            int x = hp_digits.at(i);
            SDL_Rect hb = {(w / 2 - offset) + (i * 100), h / 2 - 700, 100, 100};
            TextureType tex;
            tex = getTextureType(x);
            auto digit = EntityFactory::createGameObject(tex, hb);
            render_handler.includeInRender(std::move(digit), 1);
        }

        std::vector<int> level_digits;
        int level = _level;
        while (level > 0) {
            level_digits.push_back(level % 10);
            level /= 10;
        }
        std::reverse(level_digits.begin(), level_digits.end());
        if (level_digits.size() == 0) {
            level_digits.push_back(0);
        }
        offset = (level_digits.size() / 2.0) * 100;
        for (int i = 0; i < level_digits.size(); ++i) {
            int x = level_digits.at(i);
            SDL_Rect hb = {(w / 2 - offset) + (i * 100), h / 2 - 600, 100, 100};
            TextureType tex;
            tex = getTextureType(x);
            auto digit = EntityFactory::createGameObject(tex, hb);
            render_handler.includeInRender(std::move(digit), 1);
        }
    }
}

std::string DeathMenu::handleMenu(RenderHandler& render_handler) {
    _ui.at("restart")->setTexture(TextureType::restart);
    _ui.at("replay")->setTexture(TextureType::replay);
    _ui.at("exit")->setTexture(TextureType::exit);
    if (_message.find("e") != std::string::npos) {
        // Enter pressed
        if (_state == DeathStates::Restart) {
            return "restart";
            _finished = true;
        } else if (_state == DeathStates::Replay) {
            return "replay";
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
    } else if (_state == DeathStates::Replay) {
        _ui.at("replay")->setTexture(TextureType::replay_hovered);
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
                FileHandler::getInstance().saveResolution(1280, 720);
            } else if (_resolution == Resolutions::p1920x1080) {
                render_handler.setRenderingSize(1920, 1080);
                FileHandler::getInstance().saveResolution(1920, 1080);
            } else if (_resolution == Resolutions::p2560x1440) {
                render_handler.setRenderingSize(2560, 1440);
                FileHandler::getInstance().saveResolution(2560, 1440);
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
    Menu::includeInRender(render_handler);
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
        tex = getTextureType(letter);
        auto let = EntityFactory::createGameObject(tex, hb);
        render_handler.includeInRender(std::move(let), 1);
    }
}
std::string LoginMenu::getName() { return _name; }

void LeaderboardMenu::init() {
    auto size = RenderHandler::getSize();
    int w = size.first;
    int h = size.second;
    std::shared_ptr<GameObject> exit = EntityFactory::createGameObject(
        TextureType::exit_hovered, {w / 2 - 400, h / 2 + 400, 800, 200});
    _ui.insert({"exit", std::move(exit)});
    _leaderboard = FileHandler::getInstance().loadScores();
    std::reverse(_leaderboard.begin(), _leaderboard.end());
}

std::string LeaderboardMenu::handleMenu(RenderHandler& render_handler) {
    _ui.at("exit")->setTexture(TextureType::exit_hovered);
    if (_message.find("e") != std::string::npos) {
        return "exit";
    }
    _message = "";
    return "";
}

void LeaderboardMenu::includeInRender(RenderHandler& render_handler) {
    Menu::includeInRender(render_handler);
    auto size = RenderHandler::getSize();
    int w = size.first;
    int h = size.second;
    if (_leaderboard.size() == 0) {
        auto no_score = EntityFactory::createGameObject(
            TextureType::empty, {w / 2 - 400, h / 2 - 100, 800, 200});
        render_handler.includeInRender(std::move(no_score), 1);
    } else {
        for (int i = 0; i < _leaderboard.size() and i < 5; ++i) {
            auto score = _leaderboard.at(i);
            int offset = (score.name.size() / 2.0) * 100;
            for (int j = 0; j < score.name.size(); ++j) {
                char letter = score.name.at(j);
                SDL_Rect hb = {(w / 2 - offset) + (j * 100),
                               (h / 2 - 900) + (i * 200), 100, 100};
                TextureType tex;
                tex = getTextureType(letter);
                auto let = EntityFactory::createGameObject(tex, hb);
                render_handler.includeInRender(std::move(let), 1);
            }
            std::vector<int> score_digits;
            int score_int = score.score;
            while (score_int > 0) {
                score_digits.push_back(score_int % 10);
                score_int /= 10;
            }
            std::reverse(score_digits.begin(), score_digits.end());
            if (score_digits.size() == 0) {
                score_digits.push_back(0);
            }
            offset = (score_digits.size() / 2.0) * 100;
            for (int j = 0; j < score_digits.size(); ++j) {
                int x = score_digits.at(j);
                SDL_Rect hb = {(w / 2 - offset) + (j * 100),
                               (h / 2 - 800) + (i * 200), 100, 100};
                TextureType tex;
                tex = getTextureType(x);
                auto digit = EntityFactory::createGameObject(tex, hb);
                render_handler.includeInRender(std::move(digit), 1);
            }
        }
    }
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

std::shared_ptr<LeaderboardMenu> UIFactory::createLeaderboardMenu() {
    return std::make_shared<LeaderboardMenu>();
}
