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

class Scoreboard {
   private:
    int _score = 0;
    std::string getDigitPath(int scoreDigit);

   public:
    void operator++(int x);
    void operator++();
    void operator+=(unsigned inc);
    void operator-=(unsigned dec);
    int getScore();
    SDL_Texture* getDigitTexture(int digit, SDL_Renderer* ren);
};

class InputListener {
   public:
    virtual void handleInput(const std::string message, float delta_time,
                             RenderHandler* render_handler,
                             Scoreboard* scoreboard) = 0;
};

class InputHandler {
   protected:
    std::list<std::shared_ptr<InputListener>> _subscribers;
    void notifySubs(std::string message, float delta_time,
                    RenderHandler* render_handler, Scoreboard* scoreboard);

   public:
    InputHandler();
    void subscribe(std::shared_ptr<InputListener> observer);
    std::string handleInput(float delta_time, RenderHandler* render_handler,
                            Scoreboard* scoreboard);
};

class GameObject {
   protected:
    SDL_Rect _hitbox = {};
    SDL_Texture* _texture = nullptr;

   public:
    SDL_Texture* getTexture();
    SDL_Rect* getHitbox();
    void setTexture(const std::string path, SDL_Renderer* ren);
    virtual ~GameObject();
    GameObject(SDL_Rect hitbox);
};

class Entity : public GameObject {
   protected:
    int _hp = 1000;

   public:
    Entity(SDL_Rect hitbox, int hp);
    virtual void hit(int damage);
    int getHP();
};

class ControlledEntity : public Entity {
   protected:
    unsigned _velocity;
    CollisionHandler* _collision_handler;
    virtual void attackLeft(RenderHandler* render_handler);
    virtual void attackRight(RenderHandler* render_handler);
    virtual void attackUp(RenderHandler* render_handler);
    virtual void attackDown(RenderHandler* render_handler);

   public:
    ControlledEntity(SDL_Rect hitbox, int hp, unsigned velocity);
    void addCollisionHandler(CollisionHandler* collision_handler);
};

class Player : public ControlledEntity, public InputListener {
   private:
    int _attack_frames = 7;
    int _damage = 5;
    void handleHit(RenderHandler* render_handler, Scoreboard* scoreboard,
                   SDL_Rect attack_hitbox);
    void hitLeft(RenderHandler* render_handler, Scoreboard* scoreboard);
    void hitRight(RenderHandler* render_handler, Scoreboard* scoreboard);
    void hitUp(RenderHandler* render_handler, Scoreboard* scoreboard);
    void hitDown(RenderHandler* render_handler, Scoreboard* scoreboard);

   public:
    Player(SDL_Rect hitbox, int hp, unsigned velocity);
    void handleInput(const std::string message, float delta_time,
                     RenderHandler* render_handler,
                     Scoreboard* scoreboard) override;
};

class Laboratory : public Entity {
   private:
    unsigned _animals_stored = 1;

   public:
    Laboratory(SDL_Rect hitbox, int hp, unsigned animals_stored);
    bool checkDistanceToPlayer(const SDL_Rect player, unsigned threshold);
    unsigned getAnimalCount();
};

class EntityFactory {
   public:
    static std::unique_ptr<GameObject> createGameObject(const std::string path,
                                                        SDL_Renderer* ren,
                                                        SDL_Rect hitbox);
    static std::unique_ptr<Entity> createEntity(const std::string path,
                                                SDL_Renderer* ren,
                                                SDL_Rect hitbox, int hp);
    static std::unique_ptr<ControlledEntity> createControlledEntity(
        const std::string path, SDL_Renderer* ren, SDL_Rect hitbox, int hp,
        unsigned velocity);

    static std::unique_ptr<Player> createPlayer(const std::string path,
                                                SDL_Renderer* ren,
                                                SDL_Rect hitbox, int hp,
                                                unsigned velocity);

    static std::unique_ptr<Laboratory> createLaboratory(
        const std::string path, SDL_Renderer* ren, SDL_Rect hitbox, int hp,
        unsigned animals_stored);
};
