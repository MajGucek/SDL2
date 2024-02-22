#pragma once

#include <SDL2x64/SDL.h>

#include <iostream>
#include <list>
#include <memory>

class Collider {
   private:
    SDL_Rect* _collider;

   public:
    Collider(SDL_Rect* collision_hitbox);
    SDL_Rect* getHitbox();
};

class CollisionHandler {
   private:
    std::list<std::shared_ptr<Collider>> _colliders;
    int _screen_width = 0;
    int _screen_height = 0;

   public:
    void setSize(int w, int h);
    void addCollider(std::shared_ptr<Collider> collider);
    bool isColliding(std::shared_ptr<Collider> collider);
};

class ColliderFactory {
   public:
    static std::unique_ptr<Collider> createCollider(SDL_Rect* collision_hitbox);
};