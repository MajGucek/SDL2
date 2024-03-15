#pragma once

#include <SDL.h>

#include <iostream>
#include <map>
#include <string>

class TimeHandler {
   private:
    int _fps;
    unsigned long _frames;
    TimeHandler();

   public:
    TimeHandler(const TimeHandler&) = delete;
    TimeHandler& operator=(const TimeHandler&) = delete;
    static TimeHandler& getInstance();
    void setFramerate(int fps);
    void handleFramerate();
    void updateFrame();
    unsigned long getTime();
    float deltaTime();
};

class InternalTimer {
   private:
    bool _exist = false;
    unsigned long _start = 0;
    int _frames = -1;
    bool finished();

   public:
    void startTimer(int frames);
    bool exists();
};