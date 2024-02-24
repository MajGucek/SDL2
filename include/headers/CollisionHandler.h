#pragma once

#include <Entity.h>
#include <SDL2x64/SDL.h>

#include <iostream>
#include <list>
#include <memory>

class Entity;

class CollisionHandler {
   private:
    std::list<std::weak_ptr<Entity>> _colliders;
    int _screen_width = 0;
    int _screen_height = 0;

   public:
    void setSize(int w, int h);
    void addCollider(std::weak_ptr<Entity> collider);
    std::pair<std::weak_ptr<Entity>, bool> isColliding(SDL_Rect* collider);
};
