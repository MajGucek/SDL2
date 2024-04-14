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
    void resetScore();
    void setScore(int score);
    int getScore();
    SDL_Texture* getDigitTexture(int digit, SDL_Renderer* ren);
};
class InputListener {
   protected:
    std::string _message;

   public:
    virtual bool handleInput(const std::string message);
};
class InputHandler {
   private:
    std::list<std::weak_ptr<InputListener>> _subscribers;
    bool notifySubs(std::string message);
    InternalTimer _delay;
    std::string _message;

   public:
    void subscribe(std::weak_ptr<InputListener> observer);
    void deleteSubs();
    bool handleInput();
    void addDelay(int ms);
    std::string getInput();
};

class GameObject {
   protected:
    SDL_Rect _hitbox;
    TextureType _texture_type;

   public:
    GameObject(SDL_Rect hitbox);
    virtual SDL_Texture* getTexture();
    virtual SDL_Rect* getHitbox();
    virtual void setTexture(TextureType texture_type);
};

class Entity : public GameObject {
   protected:
    InternalTimer _invincibility_timer;
    InternalTimer _animation_timer;
    int _invincibility_time = 5;
    const int _hit_animation_time = 10;
    int _hp = 100;

   public:
    Entity(SDL_Rect hitbox, int hp);
    virtual void hit(int damage, RenderHandler* render_handler);
    int getHP();
};

class ControlledEntity : public Entity {
   protected:
    InternalTimer _attack_timer;
    InternalTimer _movement_timer;
    const int _movement_time = 5000;
    const int _attack_delay_time = 5;
    int _damage = 25;
    int _velocity = 5;
    CollisionHandler* _collision_handler;

   public:
    int getDamage();
    void setPos(std::pair<int, int> pos);
    ControlledEntity(SDL_Rect hitbox, int hp, int velocity);
    void addCollisionHandler(CollisionHandler* collision_handler);
    virtual void handle(RenderHandler* render_handler, float delta_time) {}
};

class Player : public ControlledEntity, public InputListener {
   private:
    const int _movement_time = 3800;
    const int _attack_delay_time = 400;
    const int _attack_animation_time = 25;
    const int _attack_size = 300;
    int _damage = 50;
    int _invincibility_time = 70;
    void handleHit(RenderHandler* render_handler, SDL_Rect attack_hitbox);
    void hitLeft(RenderHandler* render_handler);
    void hitRight(RenderHandler* render_handler);
    void hitUp(RenderHandler* render_handler);
    void hitDown(RenderHandler* render_handler);

   public:
    void hit(int damage, RenderHandler* render_handler) override;
    Player() = default;
    Player(SDL_Rect hitbox, int hp, int velocity);
    bool handleInput(const std::string message) override;
    void handle(RenderHandler* render_handler, float delta_time) override;
    void setHP(int hp);
};

class Poacher : public ControlledEntity {
   private:
    const int _score = 100;
    const int _attack_time = 30;
    const int _death_time = 5;
    int _damage = 25;
    int _hp = 1;

    void handleCollisionHelper(int x, int y);

   public:
    bool canAttack(RenderHandler* render_handler);
    Poacher(SDL_Rect hitbox, int hp, int velocity);
    bool canSeePlayer(const SDL_Rect player, unsigned threshold);
    void moveTowards(const SDL_Rect destination, float delta_time);
    int getScore();
    void hit(int damage, RenderHandler* render_handler) override;
};

class Pangolin : public ControlledEntity {
   private:
    InternalTimer _slam;
    const int _damage = 30;
    const int _slam_delay = 500;
    const int _rune_time = 400;
    const int _score = 500;

   public:
    Pangolin(SDL_Rect hitbox, int hp, int velocity, int damage);
    void attack(RenderHandler* render_handler);
};

class Laboratory : public Entity {
   private:
    const int _hit_animation_time = 10;
    unsigned _animals_stored = 500;

   public:
    void hit(int damage, RenderHandler* render_handler) override;
    SDL_Texture* getTexture() override;
    Laboratory(SDL_Rect hitbox, int hp, unsigned animals_stored);
    bool checkDistanceToPlayer(SDL_Rect player, unsigned threshold);
    unsigned getAnimalCount();
};

class EntityFactory {
   public:
    static std::unique_ptr<GameObject> createGameObject(
        TextureType texture_type, SDL_Rect hitbox);
    static std::unique_ptr<ControlledEntity> createGameControlledEntity(
        TextureType texture_type, SDL_Rect hitbox, int velocity);

    static std::unique_ptr<Player> createPlayer(SDL_Rect hitbox, int hp,
                                                int velocity);
    static std::unique_ptr<Poacher> createPoacher(SDL_Rect hitbox, int hp,
                                                  int velocity);

    static std::unique_ptr<Laboratory> createLaboratory(
        SDL_Rect hitbox, int hp, unsigned animals_stored);
};
