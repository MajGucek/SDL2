#pragma once

#include <SDL2x64/SDL.h>

#include <list>

class Collider {
   private:
    SDL_Rect* _collider;

   public:
    Collider(SDL_Rect* collision_hitbox);
    SDL_Rect* getHitbox();
};

class CollisionHandler {
   private:
    std::list<Collider*> _colliders;

   public:
    void addCollider(Collider* collider);
    bool isColliding(Collider* collider);
};

class ColliderFactory {
   public:
    static Collider* createCollider(SDL_Rect* collision_hitbox);
    static Collider createColliderOnStack(SDL_Rect* collision_hitbox);
};