#include <TimeHandler.h>

TimeHandler::TimeHandler() : _last(0), _now(0) {}

TimeHandler& TimeHandler::getInstance() {
    static TimeHandler timer;
    return timer;
}

float TimeHandler::deltaTime() {
    return (float)((_now - _last) * 1000 /
                   (double)SDL_GetPerformanceFrequency());
}

void TimeHandler::tick() {
    _last = _now;
    _now = SDL_GetPerformanceCounter();
}

// Internal Timer //
InternalTimer::InternalTimer() : _exist(false), _start_time(0), _duration(0) {}
void InternalTimer::startTimer(int ms) {
    _exist = true;
    _duration = ms;
    _start_time = SDL_GetTicks64();
}
bool InternalTimer::exists() {
    if (finished()) {
        _exist = false;
    }
    return _exist;
}

bool InternalTimer::finished() {
    if (!_exist) {
        return true;
    }
    if (SDL_GetTicks64() - _start_time >= _duration) {
        _exist = false;
        return true;
    } else {
        return false;
    }
}
