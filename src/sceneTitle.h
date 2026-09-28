#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include "scene.h"

class SceneTitle : public Scene
{
public:
    SceneTitle();
    ~SceneTitle();

    void init() override;
    void update(double delta_time) override;
    void render() override;
    void clean() override;
    void handleEvents(SDL_Event* event) override;

private:
    MIX_Audio* music_data = nullptr;
    float timer = 0.0f;
};



#endif // SCENE_TITLE_H