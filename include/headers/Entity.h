#pragma once

#include <CollisionHandler.h>
#include <RenderHandler.h>
#include <SDL2x64/SDL.h>
#include <SDL2x64/SDL_image.h>

class CollisionHandler;
class RenderHandler;

#include <cmath>
#include <iostream>
#include <memory>
#include <string>

class InputListener {
   public:
    virtual void handleInput(const std::string message, float delta_time,
                             RenderHandler* render_handler) = 0;
};

class InputHandler {
   protected:
    std::list<std::shared_ptr<InputListener>> _subscribers;
    void notifySubs(std::string message, float delta_time,
                    RenderHandler* render_handler);

   public:
    InputHandler();
    void subscribe(std::shared_ptr<InputListener> observer);
    std::string handleInput(float delta_time, RenderHandler* render_handler);
};

class Entity {
   protected:
    int _hp = 1000;
    SDL_Texture* _texture = nullptr;
    SDL_Rect _hitbox = {};

   public:
    Entity() = default;
    Entity(SDL_Rect hitbox);
    virtual ~Entity();
    void setTexture(const std::string path, SDL_Renderer* ren);
    SDL_Texture* getTexture();
    SDL_Rect* getHitbox();
    virtual void hit(int damage);
    int getHP();
};
class ControlledEntity : public Entity {
   protected:
    unsigned _velocity;
    CollisionHandler* _collision_handler;
    virtual void hitLeft(RenderHandler* render_handler);
    virtual void hitRight(RenderHandler* render_handler);
    virtual void hitUp(RenderHandler* render_handler);
    virtual void hitDown(RenderHandler* render_handler);

   public:
    ControlledEntity(SDL_Rect hitbox, unsigned velocity);
    void addCollisionHandler(CollisionHandler* collision_handler);
};

class Player : public ControlledEntity, public InputListener {
   protected:
    int _damage = 5;
    void hitLeft(RenderHandler* render_handler) override;
    void hitRight(RenderHandler* render_handler) override;
    void hitUp(RenderHandler* render_handler) override;
    void hitDown(RenderHandler* render_handler) override;

   public:
    Player(SDL_Rect hitbox, unsigned velocity);
    void handleInput(const std::string message, float delta_time,
                     RenderHandler* render_handler) override;
};

class Laboratory : public Entity {
   private:
    unsigned animals_stored;

   public:
    Laboratory(SDL_Rect hitbox);
    bool checkDistanceToPlayer(const SDL_Rect player, unsigned threshold);
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
