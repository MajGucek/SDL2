#pragma once

#include <SDL2x64/SDL.h>

#include <list>
#include <memory>
#include <string>

// Observer: == Entity

class InputListener {
   public:
    virtual void handleInput(const std::string message, float delta_time) = 0;
};

class InputHandler {
   protected:
    std::list<std::shared_ptr<InputListener>> _subscribers;
    void notifySubs(std::string message, float delta_time);

   public:
    InputHandler();
    void subscribe(std::shared_ptr<InputListener> observer);
    std::string handleInput(float delta_time);
};
