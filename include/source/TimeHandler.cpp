#include <TimeHandler.h>

TimeHandler::TimeHandler() : _fps(60), _frames(0) {}

TimeHandler& TimeHandler::getInstance() {
    static TimeHandler timer;
    return timer;
}

void TimeHandler::setFramerate(int fps) { _fps = fps; }

void TimeHandler::handleFramerate() {
    int desiredDelta = 1000 / _fps;
    int startLoop = SDL_GetTicks();
    int delta = SDL_GetTicks() - startLoop;
    if (delta < desiredDelta) {
        SDL_Delay(desiredDelta - delta);
    }
}

void TimeHandler::updateFrame() { _frames++; }

unsigned long TimeHandler::getTime() { return _frames; }

void InternalTimer::startTimer(int frames) {
    _exist = true;
    _frames = frames;
    _start = TimeHandler::getInstance().getTime();
}

bool InternalTimer::exists() {
    if (finished()) {
        _exist = false;
    }
    return _exist;
}

bool InternalTimer::finished() {
    if (TimeHandler::getInstance().getTime() >= _start + _frames) {
        _start = 0;
        _frames = -1;
        _exist = false;
        return true;
    } else {
        return false;
    }
}

float TimeHandler::deltaTime() { return (double)1 / (double)_fps; }