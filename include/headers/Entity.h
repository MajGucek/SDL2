#pragma once

#include <CollisionHandler.h>
#include <InputHandler.h>
#include <SDL2x64/SDL.h>
#include <SDL2x64/SDL_image.h>

#include <cmath>
#include <iostream>
#include <memory>
#include <string>

class Entity {
   protected:
    SDL_Texture* _texture;
    SDL_Rect _hitbox;
    std::shared_ptr<Collider> _collider;

   public:
    Entity() = default;
    Entity(SDL_Rect hitbox);
    virtual ~Entity();
    void setTexture(const std::string path, SDL_Renderer* ren);
    SDL_Texture* getTexture();
    SDL_Rect* getHitbox();

    virtual void isCollider(CollisionHandler* collision_handler);
};
class ControlledEntity : public Entity {
   protected:
    unsigned _velocity;
    CollisionHandler* _collision_handler;

   public:
    ControlledEntity(SDL_Rect hitbox, unsigned velocity);

    virtual void isCollider(CollisionHandler* collision_handler) override;
};

class Player : public ControlledEntity, public InputListener {
   public:
    Player(SDL_Rect hitbox, unsigned velocity);
    virtual void handleInput(const std::string message,
                             float delta_time) override;
};

class Laboratory : public Entity {
   private:
    unsigned animals_stored;
    std::list<std::weak_ptr<Laboratory>> _connections;

   public:
    Laboratory(SDL_Rect hitbox);
    bool checkDistanceToPlayer(const SDL_Rect player, unsigned threshold);
    void connectedTo(std::weak_ptr<Laboratory> laboratory);
};

class EntityFactory {
   public:
    static std::unique_ptr<Entity> createEntity(const std::string path,
                                                SDL_Renderer* ren,
                                                SDL_Rect hitbox);
    static std::unique_ptr<ControlledEntity> createControlledEntity(
        const std::string path, SDL_Renderer* ren, SDL_Rect hitbox,
        unsigned velocity);
    static std::unique_ptr<Player> createPlayer(const std::string path,
                                                SDL_Renderer* ren,
                                                SDL_Rect hitbox,
                                                unsigned velocity);
    static std::unique_ptr<Laboratory> createLaboratory(const std::string path,
                                                        SDL_Renderer* ren,
                                                        SDL_Rect hitbox);
};
