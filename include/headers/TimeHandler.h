#pragma once

#include <SDL2x64/SDL.h>

#include <iostream>
#include <map>
#include <string>

class TimeHandler {
   private:
    int _fps = 0;
    unsigned long _frames = 0;
    std::map<std::string, std::pair<unsigned long, int>> _timers;
    TimeHandler() = default;

   public:
    TimeHandler(const TimeHandler&) = delete;
    TimeHandler& operator=(const TimeHandler&) = delete;
    static TimeHandler& getInstance();
    void setFramerate(int fps);
    void handleFramerate();
    void updateFrame();
    void addTimer(std::string timer_name, int frames);
    bool timerExist(std::string timer_name);
    // bool hasPassedFrames(std::string timer_name, int frames);
};