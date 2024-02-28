#include <TimeHandler.h>

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

void TimeHandler::updateFrame() {
    _frames++;
    for (auto& time : _timers) {
        if (_frames >= time.second.first + time.second.second) {
            // potekel cas timerja
            _timers.erase(time.first);
        }
    }
}

void TimeHandler::addTimer(std::string timer_name, int frames) {
    _timers.insert({timer_name, {_frames, frames}});
}

/*
bool TimeHandler::hasPassedFrames(std::string timer_name, int frames) {
    if (auto time = _timers.find(timer_name); time != _timers.end()) {
        if (_frames < time->second + frames) {
            _timers.erase(timer_name);
            return true;
        } else {
            return false;
        }
    }
}
*/

bool TimeHandler::timerExist(std::string timer_name) {
    if (_timers.find(timer_name) != _timers.end()) {
        return true;
    } else {
        return false;
    }
}