#pragma once

#include <SDL.h>

#include <iostream>
#include <map>
#include <string>

class TimeHandler {
   private:
    Uint64 _last;
    Uint64 _now;
    TimeHandler();

   public:
    TimeHandler(const TimeHandler&) = delete;
    TimeHandler& operator=(const TimeHandler&) = delete;
    static TimeHandler& getInstance();
    float deltaTime();
    void tick();
};

class InternalTimer {
   private:
    bool _exist = false;
    Uint64 _start_time = 0;
    Uint64 _duration = 0;  // ms
    bool finished();

   public:
    InternalTimer();
    void startTimer(int ms);
    bool exists();
};