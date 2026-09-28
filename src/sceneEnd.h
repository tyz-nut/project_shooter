#ifndef SCENE_END_H
#define SCENE_END_H

#include "scene.h"

class SceneEnd : public Scene {
public:
    SceneEnd();
    ~SceneEnd();
    void init() override;
    void update(double delta_time) override;
    void render() override;
    void clean() override;
    void handleEvents(SDL_Event* event) override;

private:
    MIX_Audio* music_data = nullptr;

    bool is_type;
    std::string name = "";
    float timer = 0.0f;

    void render_phase1();
    void render_phase2();

    void remove_last_UTF8_char(std::string& str);
};
#endif 