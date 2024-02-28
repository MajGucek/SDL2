#include <UIHandler.h>

void UIHandler::initUIHandler(InputHandler* input_handler,
                              RenderHandler* render_handler) {
    if (!input_handler) {
        std::cout << "no input handler!" << std::endl;
        return;
    }
    if (!render_handler) {
        std::cout << "no render handler!" << std::endl;
        return;
    }
    _input_handler = input_handler;
    _render_handler = render_handler;
}
void UIHandler::addUIElement(std::shared_ptr<UIButton> ui) {
    _ui.push_back(std::move(ui));
}

bool UIHandler::handleUI() {
    SDL_MouseButtonEvent mouse = _input_handler->getMouse();
    for (auto& ui : _ui) {
        if (ui->handleButton(mouse.x, mouse.y, mouse.type, mouse.button,
                             _render_handler)) {
            //  clicked button
            _ui.clear();
            return true;
        }
    }
    return false;
}