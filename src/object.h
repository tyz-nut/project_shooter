#ifndef OBJECT_H
#define OBJECT_H

#include <SDL3/SDL.h>

enum class ItemType
{
    Health,
    Shield,
    Time
};

struct Player
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    float width = 0;
    float height = 0;
    int speed = 300;
    int current_hp = 5;
    int max_hp = 5;
    Uint64 cool_down = 500;
    Uint64 last_shoot = 0;
};

struct Enemy
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    float width = 0;
    float height = 0;
    int speed = 150;
    int current_hp = 2;
    int damage = 1;
    Uint64 cool_down = 3000;
    Uint64 last_shoot = 0;
};

struct ProjectilePlayer
{
    SDL_Texture *texture = nullptr;
    SDL_FPoint position = {0, 0};
    float width = 0;
    float height = 0;
    int speed = 400;
    int damage = 1;
};

struct ProjectileEnemy
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    SDL_FPoint direction = {0, 0};
    float width = 0;
    float height = 0;
    int speed = 400;
    int damage = 1;
};

struct Explosion
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    float width = 0;
    float height = 0;
    int current_frame = 0;
    int total_frames = 10;
    Uint32 start_time = 0;
    Uint32 FPS = 10;
};

struct Item
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    SDL_FPoint direction = {0, 0};
    int speed = 200;
    float width = 0;
    float height = 0;
    int bounce = 3;
    ItemType type = ItemType::Health;
};

struct Background
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int speed = 0;
    float width = 0;
    float height = 0;
    float offset = 0;
};



#endif // OBJECT_H