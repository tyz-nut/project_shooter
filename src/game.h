#ifndef GAME_H
#define GAME_H

#include "scene.h"
#include "object.h"
#include <map>

class Game
{
public:
    static Game& getInstance()
    {
        static Game instance;
        return instance;
    }
    ~Game();

    void run();
    void init();
    void clean();
    void changeScene(Scene* scene);

    void handleEvents(SDL_Event* event);
    void update();
    void render();

    
    void update_background();
    void render_background();

    void save_data();
    void load_data();

    // 渲染工具函数
    SDL_FRect render_text_center(const std::string& text, float pos_y, bool is_title = false);
    SDL_FRect render_text_pos(const std::string& text, float pos_x, float pos_y, bool is_left = true);

    void setFinalScore(int score) { final_score = score; }
    int getFinalScore() const { return final_score; }

    void insert_leaderboard(int score, const std::string& name);

    int getWindowWidth() const { return window_width; }
    int getWindowHeight() const { return window_height; }
    SDL_Window* getWindow() const { return window; }
    SDL_Renderer* getRenderer() const { return renderer; }
    MIX_Mixer* getMixer() const { return mixer; }
    MIX_Track* getMusicTrack() const { return music_track; }
    std::multimap<int, std::string, std::greater<int>>& getLeaderboard() { return leaderboard; }

private:
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    bool is_running = true;
    bool is_fullscreen = false;
    int final_score = 0;
    std::multimap<int, std::string, std::greater<int>> leaderboard;
    Scene* current_scene = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    MIX_Mixer* mixer = nullptr;
    MIX_Track* music_track = nullptr;

    TTF_Font* title_font = nullptr;
    TTF_Font* text_font = nullptr;

    int window_width = 600;
    int window_height = 800;
    int fps = 60;
    Uint64 frame_time = 1000 / fps;
    double delta_time = 0.0;

    Background near_star;
    Background far_star;
};

#endif // GAME_H