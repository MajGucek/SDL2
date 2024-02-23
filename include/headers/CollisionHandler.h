#pragma once

#include <SDL2x64/SDL.h>

#include <iostream>
#include <list>
#include <memory>

class CollisionHandler {
   private:
    std::list<SDL_Rect*> _colliders;
    int _screen_width = 0;
    int _screen_height = 0;

   public:
    void setSize(int w, int h);
    void addCollider(SDL_Rect* collider);
    bool isColliding(SDL_Rect* collider);
};
