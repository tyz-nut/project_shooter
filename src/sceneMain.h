#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#include <vector>
#include <random>
#include <map>

#include "scene.h"
#include "object.h"

class SceneMain : public Scene
{
public:
    SceneMain();
    ~SceneMain();

    void init() override;
    void update(double delta_time) override;
    void render() override;
    void clean() override;
    void handleEvents(SDL_Event *event) override;

    SDL_FPoint get_direction(Enemy *enemy);

private:
    Player player;

    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    bool is_dead = false;
    int score = 0;
    float timer_end = 0;

    MIX_Audio* music_data = nullptr;
    std::map <std::string, MIX_Audio*> sounds;
    TTF_Font* score_font = nullptr;

    SDL_Texture* ui_health = nullptr;

    // 创建对象模版文件
    Enemy enemy_template;
    ProjectilePlayer projectile_player_template;
    ProjectileEnemy projectile_enemy_template;
    Explosion explosion_template;
    Item item_life_template;

    // 创建对象容器
    std::vector<Enemy*> enemies;
    std::vector<ProjectilePlayer*> projectiles_player;
    std::vector<ProjectileEnemy*> projectiles_enemy;
    std::vector<Explosion*> explosions;
    std::vector<Item*> items;

    // 渲染函数
    void render_enemies();
    void render_projectiles_player();
    void render_projectiles_enemy();
    void render_explosions();
    void render_items();
    void render_ui();

    // 更新函数
    void update_player();
    void update_enemies(double delta_time);
    void update_projectiles_player(double delta_time);
    void update_projectiles_enemy(double delta_time);
    void update_explosions(double delta_time);
    void update_items(double delta_time);

    // 其他
    void keyPressedControl(double delta_time);
    void shoot_player();
    void shoot_enemy(Enemy *enemy);
    void spawn_enemy();
    void enemy_explode(Enemy *enemy);
    void drop_item(Enemy *enemy);
    void player_get_item(Item *item);
};
#endif
