#pragma once

#include <AudioHandler.h>
#include <CollisionHandler.h>
#include <RenderHandler.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <TimeHandler.h>
class CollisionHandler;
class RenderHandler;
enum class TextureType;

#include <cmath>
#include <iostream>
#include <list>
#include <memory>
#include <string>

class Scoreboard {
   private:
    Scoreboard() = default;
    int _score = 0;
    std::string getDigitPath(int scoreDigit);

   public:
    Scoreboard(const Scoreboard&) = delete;
    Scoreboard& operator=(const Scoreboard&) = delete;
    static Scoreboard& getInstance();
    void operator++(int x);
    void operator++();
    void operator+=(unsigned inc);
    void operator-=(unsigned dec);
    int getScore();
    SDL_Texture* getDigitTexture(int digit, SDL_Renderer* ren);
};
class InputListener {
   public:
    virtual bool handleInput(const std::string message,
                             RenderHandler* render_handler) = 0;
};
class InputHandler {
   private:
    std::list<std::shared_ptr<InputListener>> _subscribers;
    bool notifySubs(std::string message, RenderHandler* render_handler);

   public:
    InputHandler();
    void subscribe(std::shared_ptr<InputListener> observer);
    bool handleInput(RenderHandler* render_handler);
};

class GameObject {
   protected:
    SDL_Rect _hitbox;
    TextureType _texture_type;
    SDL_Texture* _texture = nullptr;

   public:
    GameObject(SDL_Rect hitbox);
    virtual ~GameObject();
    virtual SDL_Texture* getTexture();
    virtual SDL_Rect* getHitbox();
    virtual void setTexture(TextureType texture_type);
};

class Entity : public GameObject {
   protected:
    InternalTimer _invincibility_timer;
    InternalTimer _animation_timer;
    int _invincibility_frames = 5;
    const int _hit_animation_frames = 10;
    int _hp = 100;

   public:
    Entity(SDL_Rect hitbox, int hp);
    virtual void hit(int damage, SDL_Renderer* renderer,
                     std::string audio = "");
    int getHP();
};

class ControlledEntity : public Entity {
   protected:
    InternalTimer _attack_timer;
    const int _attack_frames = 5;
    const int _damage = 5;
    unsigned _velocity = 5;
    CollisionHandler* _collision_handler;
    virtual void attackLeft(RenderHandler* render_handler);
    virtual void attackRight(RenderHandler* render_handler);
    virtual void attackUp(RenderHandler* render_handler);
    virtual void attackDown(RenderHandler* render_handler);

   public:
    int getDamage();
    ControlledEntity(SDL_Rect hitbox, int hp, unsigned velocity);
    void addCollisionHandler(CollisionHandler* collision_handler);
};

class Player : public ControlledEntity, public InputListener {
   private:
    const int _attack_frames = 25;
    int _invincibility_frames = 70;
    void handleHit(RenderHandler* render_handler, SDL_Rect attack_hitbox);
    void hitLeft(RenderHandler* render_handler);
    void hitRight(RenderHandler* render_handler);
    void hitUp(RenderHandler* render_handler);
    void hitDown(RenderHandler* render_handler);

   public:
    void hit(int damage, SDL_Renderer* renderer, std::string audio = "");
    Player(SDL_Rect hitbox, int hp, unsigned velocity);
    bool handleInput(const std::string message,
                     RenderHandler* render_handler) override;
};

class Poacher : public ControlledEntity {
   private:
    const int _damage = 10;
    const int _score = 100;
    const int _attack_frames = 30;

    void handleCollisionHelper(int x, int y);

   public:
    bool attack(RenderHandler* render_handler);
    Poacher(SDL_Rect hitbox, int hp, unsigned velocity);
    bool canSeePlayer(const SDL_Rect player, unsigned threshold);
    void moveTowards(const SDL_Rect destination);
    int getScore();
};

class Laboratory : public Entity {
   private:
    const int _hit_animation_frames = 10;
    unsigned _animals_stored = 500;

   public:
    void hit(int damage, SDL_Renderer* renderer,
             std::string audio = "") override;
    SDL_Texture* getTexture() override;
    Laboratory(SDL_Rect hitbox, int hp, unsigned animals_stored);
    bool checkDistanceToPlayer(const SDL_Rect player, unsigned threshold);
    unsigned getAnimalCount();
};
class EntityFactory {
   public:
    static std::unique_ptr<GameObject> createGameObject(
        TextureType texture_type, SDL_Renderer* ren, SDL_Rect hitbox);

    static std::unique_ptr<Player> createPlayer(SDL_Renderer* ren,
                                                SDL_Rect hitbox, int hp,
                                                unsigned velocity);
    static std::unique_ptr<Poacher> createPoacher(SDL_Renderer* ren,
                                                  SDL_Rect hitbox, int hp,
                                                  unsigned velocity);

    static std::unique_ptr<Laboratory> createLaboratory(
        SDL_Renderer* ren, SDL_Rect hitbox, int hp, unsigned animals_stored);
};
