#ifndef SCENE_H
#define SCENE_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <string>

class Game;

class Scene {
public:
    Scene();
    virtual ~Scene() = default;

    virtual void init() = 0;
    virtual void update(double delta_time) = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
    virtual void handleEvents(SDL_Event* event) = 0;
protected:
    Game& game;
};

#endif // SCENE_H