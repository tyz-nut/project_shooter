#include "sceneMain.h"
#include "sceneTitle.h"
#include "sceneEnd.h"
#include "game.h"

SceneMain::SceneMain()
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::init()
{
    // 随机数种子
    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

    // 加载音乐
    {
        music_data = MIX_LoadAudio(game.getMixer(), "assets/music/03_Racing_Through_Asteroids_Loop.ogg", false);
        if (music_data == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载音乐失败: %s", SDL_GetError());
        }
        MIX_SetTrackAudio(game.getMusicTrack(), music_data);
        MIX_SetTrackGain(game.getMusicTrack(), 0.1f);
        SDL_PropertiesID play_props = SDL_CreateProperties();               // 创造一个空的属性容器
        SDL_SetNumberProperty(play_props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);  // 设置循环次数为-1，表示无限循环
        MIX_PlayTrack(game.getMusicTrack(), play_props);
        SDL_DestroyProperties(play_props);                                  // 播放后可以销属性容器
    }

    // 加载音效
    {
        sounds["player_shoot"] = MIX_LoadAudio(game.getMixer(), "assets/sound/laser_shoot4.wav", true);
        if (sounds["player_shoot"] == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载音效失败: %s", SDL_GetError());
        }
    
        sounds["enemy_shoot"] = MIX_LoadAudio(game.getMixer(), "assets/sound/xs_laser.wav", true);
        if (sounds["enemy_shoot"] == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载音效失败: %s", SDL_GetError());
        }
    
        sounds["enemy_explosion"] = MIX_LoadAudio(game.getMixer(), "assets/sound/explosion3.wav", true);
        if (sounds["enemy_explosion"] == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载音效失败: %s", SDL_GetError());
        }
    
        sounds["player_explosion"] = MIX_LoadAudio(game.getMixer(), "assets/sound/explosion1.wav", true);
        if (sounds["player_explosion"] == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载音效失败: %s", SDL_GetError());
        }
    
        sounds["hit"] = MIX_LoadAudio(game.getMixer(), "assets/sound/eff11.wav", true);
        if (sounds["hit"] == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载音效失败: %s", SDL_GetError());
        }
    
        sounds["item"] = MIX_LoadAudio(game.getMixer(), "assets/sound/eff5.wav", true);
        if (sounds["item"] == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载音效失败: %s", SDL_GetError());
        }
    }

    // 载入字体
    score_font = TTF_OpenFont("assets/font/VonwaonBitmap-12px.ttf", 24);
    if (score_font == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载字体失败: %s", SDL_GetError());
    }

    ui_health = IMG_LoadTexture(game.getRenderer(), "assets/image/Health UI Black.png");
    if (ui_health == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载贴图失败: %s", SDL_GetError());
    }

    player.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/SpaceShip.png");
    if (player.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载贴图失败: %s", SDL_GetError());
    }
    SDL_GetTextureSize(player.texture, &player.width, &player.height);
    player.width /= 5.0f;
    player.height /= 5.0f;
    player.position.x = game.getWindowWidth() / 2.0f - player.width / 2.0f;
    player.position.y = game.getWindowHeight() - player.height;

    // 初始化模板
    projectile_player_template.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/laser-1.png");
    if (projectile_player_template.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载贴图失败: %s", SDL_GetError());
    }
    SDL_GetTextureSize(projectile_player_template.texture, &projectile_player_template.width, &projectile_player_template.height);
    projectile_player_template.width /= 4.0f;
    projectile_player_template.height /= 4.0f;

    enemy_template.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/insect-2.png");
    if (enemy_template.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载贴图失败: %s", SDL_GetError());
    }
    SDL_GetTextureSize(enemy_template.texture, &enemy_template.width, &enemy_template.height);
    enemy_template.width /= 4.0f;
    enemy_template.height /= 4.0f;

    projectile_enemy_template.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bullet-2.png");
    if (projectile_enemy_template.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载贴图失败: %s", SDL_GetError());
    }
    SDL_GetTextureSize(projectile_enemy_template.texture, &projectile_enemy_template.width, &projectile_enemy_template.height);
    projectile_enemy_template.width /= 2.0f;
    projectile_enemy_template.height /= 2.0f;

    explosion_template.texture = IMG_LoadTexture(game.getRenderer(), "assets/effect/explosion.png");
    if (explosion_template.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载贴图失败: %s", SDL_GetError());
    }
    SDL_GetTextureSize(explosion_template.texture, &explosion_template.width, &explosion_template.height);
    explosion_template.total_frames = explosion_template.width / explosion_template.height;
    explosion_template.height *= 2.0f;
    explosion_template.width = explosion_template.height;

    item_life_template.texture = IMG_LoadTexture(game.getRenderer(), "assets/image/bonus_life.png");
    if (item_life_template.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载贴图失败: %s", SDL_GetError());
    }
    SDL_GetTextureSize(item_life_template.texture, &item_life_template.width, &item_life_template.height);
    item_life_template.width /= 4.0f;
    item_life_template.height /= 4.0f;
}

void SceneMain::update(double delta_time)
{
    keyPressedControl(delta_time);
    update_projectiles_player(delta_time); // 更新玩家子弹
    update_projectiles_enemy(delta_time);  // 更新敌机子弹
    spawn_enemy();                         // 生成敌机
    update_enemies(delta_time);            // 更新敌人
    update_player();                       // 更新玩家
    update_explosions(delta_time);         // 更新爆炸效果
    update_items(delta_time);              // 更新道具

    if (is_dead)
    {
        timer_end += delta_time;
        if (timer_end >= 3.0f)
        {
            game.changeScene(new SceneEnd());
        }
    }
}

void SceneMain::render()
{
    // 渲染玩家子弹
    render_projectiles_player();
    // 渲染敌人子弹
    render_projectiles_enemy();
    // 渲染玩家
    if (!is_dead)
    {
        SDL_FRect rect = {player.position.x, player.position.y, player.width, player.height};
        SDL_RenderTexture(game.getRenderer(), player.texture, nullptr, &rect);
    }
    // 渲染敌机
    render_enemies();
    // 渲染道具
    render_items();
    // 渲染爆炸效果
    render_explosions();
    // 渲染UI
    render_ui();
}

void SceneMain::clean()
{
    // 清理音乐
    MIX_StopTrack(game.getMusicTrack(), 0);
    MIX_SetTrackAudio(game.getMusicTrack(), nullptr);
    if (music_data != nullptr)
        MIX_DestroyAudio(music_data);
    
    if (score_font != nullptr)
        TTF_CloseFont(score_font);  // 关闭字体
    
    // 清理音效
    for (auto s : sounds)
    {
        if (s.second != nullptr)
            MIX_DestroyAudio(s.second);
    }
    sounds.clear();

    if (ui_health != nullptr)
        SDL_DestroyTexture(ui_health);

    // 清理容器
    for (auto p : projectiles_player)
    {
        if (p != nullptr)
            delete p;
    }
    projectiles_player.clear();

    for (auto p : enemies)
    {
        if (p != nullptr)
            delete p;
    }
    enemies.clear();

    for (auto p : projectiles_enemy)
    {
        if (p != nullptr)
            delete p;
    }
    projectiles_enemy.clear();

    for (auto p : explosions)
    {
        if (p != nullptr)
            delete p;
    }
    explosions.clear();

    for (auto p : items)
    {
        if (p != nullptr)
            delete p;
    }
    items.clear();

    // 清理模板
    if (player.texture != nullptr)
        SDL_DestroyTexture(player.texture);

    if (projectile_player_template.texture != nullptr)
        SDL_DestroyTexture(projectile_player_template.texture);

    if (enemy_template.texture != nullptr)
        SDL_DestroyTexture(enemy_template.texture);
    
    if (projectile_enemy_template.texture != nullptr)
        SDL_DestroyTexture(projectile_enemy_template.texture);
    
    if (explosion_template.texture != nullptr)
        SDL_DestroyTexture(explosion_template.texture);

    if (item_life_template.texture != nullptr)
        SDL_DestroyTexture(item_life_template.texture);
}

void SceneMain::handleEvents(SDL_Event *event)
{
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.scancode == SDL_SCANCODE_ESCAPE)
        {
            game.changeScene(new SceneTitle());
        }
    }
}

void SceneMain::keyPressedControl(double delta_time)
{
    if (is_dead)
        return;

    auto keyboardState = SDL_GetKeyboardState(nullptr);
    if (keyboardState[SDL_SCANCODE_W])
    {
        player.position.y -= player.speed * delta_time;
    }
    if (keyboardState[SDL_SCANCODE_S])
    {
        player.position.y += player.speed * delta_time;
    }
    if (keyboardState[SDL_SCANCODE_A])
    {
        player.position.x -= player.speed * delta_time;
    }
    if (keyboardState[SDL_SCANCODE_D])
    {
        player.position.x += player.speed * delta_time;
    }

    // 确保玩家不会超出屏幕边界
    if (player.position.x < 0)
    {
        player.position.x = 0;
    }
    if (player.position.x > game.getWindowWidth() - player.width)
    {
        player.position.x = game.getWindowWidth() - player.width;
    }
    if (player.position.y < 0)
    {
        player.position.y = 0;
    }
    if (player.position.y > game.getWindowHeight() - player.height)
    {
        player.position.y = game.getWindowHeight() - player.height;
    }

    // 发射子弹
    // 控制子弹发射
    if (keyboardState[SDL_SCANCODE_J])
    {
        auto current_time = SDL_GetTicks();
        if (current_time - player.last_shoot > player.cool_down)
        {
            shoot_player();
            player.last_shoot = current_time;
        }
    }
}

void SceneMain::update_player()
{
    if (is_dead)
        return;

    if (player.current_hp <= 0)
    {
        is_dead = true;
        game.setFinalScore(score);
        auto current_time = SDL_GetTicks();
        Explosion *explosion = new Explosion(explosion_template);
        explosion->position.x = player.position.x + player.width / 2.0f - explosion->width / 2.0f;
        explosion->position.y = player.position.y + player.height / 2.0f - explosion->height / 2.0f;
        explosion->start_time = current_time;
        explosions.push_back(explosion);
        MIX_PlayAudio(game.getMixer(), sounds["player_explosion"]);
        SDL_Log("Player died"); // 玩家死亡
    }
}

void SceneMain::shoot_player()
{
    // 创建子弹对象
    ProjectilePlayer *projectilePlayer = new ProjectilePlayer(projectile_player_template);
    projectilePlayer->position.x = player.position.x + player.width / 2.0f - projectile_player_template.width / 2.0f;
    projectilePlayer->position.y = player.position.y;
    projectiles_player.push_back(projectilePlayer);
    MIX_PlayAudio(game.getMixer(), sounds["player_shoot"]);
}

void SceneMain::update_projectiles_player(double delta_time)
{
    int margin = 20;
    for (auto it = projectiles_player.begin(); it != projectiles_player.end();)
    {
        auto p = *it;
        p->position.y -= p->speed * delta_time;

        if (p->position.y + p->height + margin < 0)
        {
            delete p;
            // 把当前元素与末尾交换，再删末尾
            std::swap(*it, projectiles_player.back());
            projectiles_player.pop_back();
            SDL_Log("Player projectile removed");
            continue;
        }

        SDL_FRect rect2 = {p->position.x, p->position.y, p->width, p->height}; // 碰撞检测
        for (auto e : enemies)
        {
            SDL_FRect rect = {e->position.x, e->position.y, e->width, e->height};
            if (SDL_HasRectIntersectionFloat(&rect, &rect2))
            {
                // 碰撞处理
                e->current_hp -= p->damage;
                p->position.y = -1 - margin - p->height;
                p->speed = 0;
                MIX_PlayAudio(game.getMixer(), sounds["hit"]);
                break;
            }
        }
        ++it;
    }
}

void SceneMain::render_projectiles_player()
{
    for (auto p : projectiles_player)
    {
        SDL_FRect rect = {p->position.x, p->position.y, p->width, p->height};
        SDL_RenderTexture(game.getRenderer(), p->texture, nullptr, &rect);
    }
}

void SceneMain::spawn_enemy()
{
    if (dis(gen) > 1 / 60.0f)
        return;
    Enemy* enemy = new Enemy(enemy_template);
    enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);
    enemy->position.y = - enemy->height;
    enemies.push_back(enemy);
}

void SceneMain::update_enemies(double delta_time)
{
    auto current_time = SDL_GetTicks();
    int margin = 20;
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        auto p = *it;
        if (p->current_hp <= 0)
        {
            enemy_explode(p); // 敌机爆炸
            // 把当前元素与末尾交换，再删末尾
            std::swap(*it, enemies.back());
            enemies.pop_back();
            SDL_Log("Enemy removed");
            continue;
        }

        p->position.y += p->speed * delta_time;

        if (p->position.y - margin > game.getWindowHeight())
        {
            delete p;
            // 把当前元素与末尾交换，再删末尾
            std::swap(*it, enemies.back());
            enemies.pop_back();
            SDL_Log("Enemy removed");
            continue;
        }

        SDL_FRect rect = {p->position.x, p->position.y, p->width, p->height}; // 碰撞检测
        SDL_FRect rect2 = {player.position.x, player.position.y, player.width, player.height};
        if (SDL_HasRectIntersectionFloat(&rect, &rect2) && !is_dead)
        {
            // 碰撞处理
            player.current_hp -= p->damage;
            enemy_explode(p); // 敌机爆炸
            // 把当前元素与末尾交换，再删末尾
            std::swap(*it, enemies.back());
            enemies.pop_back();
            SDL_Log("Enemy removed");
            continue;
        }

        if (p->last_shoot + p->cool_down < current_time && !is_dead)
        {
            shoot_enemy(p); // 发射子弹
            p->last_shoot = current_time; // 更新冷却时间
        }
        ++it;
    }
}

void SceneMain::enemy_explode(Enemy *enemy)
{
    auto current_time = SDL_GetTicks();
    Explosion *explosion = new Explosion(explosion_template);
    explosion->position.x = enemy->position.x + enemy->width / 2.0f - explosion->width / 2.0f;
    explosion->position.y = enemy->position.y + enemy->height / 2.0f - explosion->height / 2.0f;
    explosion->start_time = current_time;
    explosions.push_back(explosion);
    MIX_PlayAudio(game.getMixer(), sounds["enemy_explosion"]);
    if (dis(gen) > 0.5f)
        drop_item(enemy);
    score += 10;
    delete enemy;
}

void SceneMain::drop_item(Enemy *enemy)
{
    auto item = new Item(item_life_template);
    item->position.x = enemy->position.x + enemy->width / 2.0f - item_life_template.width / 2.0f;
    item->position.y = enemy->position.y + enemy->height / 2.0f - item_life_template.height / 2.0f;
    float angle = dis(gen) * 2 * M_PI;
    item->direction.x = std::cos(angle);
    item->direction.y = std::sin(angle);
    items.push_back(item);
}

void SceneMain::player_get_item(Item *item)
{
    score += 5;
    if (item->type == ItemType::Health) // 增加生命值
    {
        
        player.current_hp = std::max(player.current_hp + 1, player.max_hp);
    }
    MIX_PlayAudio(game.getMixer(), sounds["item"]);
}

void SceneMain::update_items(double delta_time)
{
    int margin = 20;
    for (auto it = items.begin(); it != items.end();)
    {
        auto p = *it;
        p->position.x += p->direction.x * p->speed * delta_time;
        p->position.y += p->direction.y * p->speed * delta_time;

        // 处理边界反弹
        if (p->bounce > 0)
        {
            if (p->position.x < 0)
            {
                p->direction.x = -p->direction.x;
                p->bounce--;
            }
            if (p->position.x + p->width > game.getWindowWidth())
            {
                p->direction.x = -p->direction.x;
                p->bounce--;
            }
            if (p->position.y < 0)
            {
                p->direction.y = -p->direction.y;
                p->bounce--;
            }
            if (p->position.y + p->height > game.getWindowHeight())
            {
                p->direction.y = -p->direction.y;
                p->bounce--;
            }
        }

        if (p->position.x + p->width + margin < 0 
            || p->position.x - margin > game.getWindowWidth() 
            || p->position.y + p->height + margin < 0 
            || p->position.y - margin > game.getWindowHeight())
        {
            delete p;
            // 把当前元素与末尾交换，再删末尾
            std::swap(*it, items.back());
            items.pop_back();
            SDL_Log("Item removed");
            continue;
        }
        
        SDL_FRect rect = {p->position.x, p->position.y, p->width, p->height};
        SDL_FRect rect2 = {player.position.x, player.position.y, player.width, player.height};
        if (SDL_HasRectIntersectionFloat(&rect, &rect2) && !is_dead)
        {
            player_get_item(p);
            delete p;
            // 把当前元素与末尾交换，再删末尾
            std::swap(*it, items.back());
            items.pop_back();
            SDL_Log("Item removed");
            continue;
        }
        ++it;
    }
}

void SceneMain::render_items()
{
    for (auto p : items)
    {
        SDL_FRect rect = {p->position.x, p->position.y, p->width, p->height};
        SDL_RenderTexture(game.getRenderer(), p->texture, nullptr, &rect);
    }
}

void SceneMain::render_ui()
{
    // 渲染血条
    float x = 10;
    float y = 10;
    float size = 25;
    float offset = 40;
    SDL_SetTextureColorMod(ui_health, 100, 100, 100);
    for (int i = 0; i < player.max_hp; ++i)
    {
        SDL_FRect rect = {x + i * offset, y, size, size};
        SDL_RenderTexture(game.getRenderer(), ui_health, nullptr, &rect);
    }
    SDL_SetTextureColorMod(ui_health, 255, 255, 255);
    for (int i = 0; i < player.current_hp; ++i)
    {
        SDL_FRect rect = {x + i * offset, y, size, size};
        SDL_RenderTexture(game.getRenderer(), ui_health, nullptr, &rect);
    }

    // 渲染得分
    auto text = "Score: " + std::to_string(score);
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(score_font, text.c_str(), 0, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(game.getRenderer(), surface);
    SDL_FRect rect = {static_cast<float>(game.getWindowWidth() - 200), 
                        10.0f, 
                        static_cast<float>(surface->w), 
                        static_cast<float>(surface->h)};
    SDL_RenderTexture(game.getRenderer(), texture, nullptr, &rect);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}

void SceneMain::render_enemies()
{
    for (auto p : enemies)
    {
        SDL_FRect rect = {p->position.x, p->position.y, p->width, p->height};
        SDL_RenderTexture(game.getRenderer(), p->texture, nullptr, &rect);
    }
}

void SceneMain::shoot_enemy(Enemy* enemy)
{
    auto projectile = new ProjectileEnemy(projectile_enemy_template);
    projectile->position.x = enemy->position.x + enemy->width / 2.0f - projectile->width / 2.0f;
    projectile->position.y = enemy->position.y + enemy->height - projectile->height / 2.0f;
    projectile->direction = get_direction(enemy); // 设置子弹方向
    projectiles_enemy.push_back(projectile); // 发射子弹
    MIX_PlayAudio(game.getMixer(), sounds["enemy_shoot"]);
}

void SceneMain::update_projectiles_enemy(double delta_time)
{
    int margin = 20;
    for (auto it = projectiles_enemy.begin(); it != projectiles_enemy.end();)
    {
        auto p = *it;
        p->position.y += p->speed * p->direction.y * delta_time;
        p->position.x += p->speed * p->direction.x * delta_time; // 更新子弹位置

        if (p->position.y + p->height + margin < 0
            || p->position.x + p->width + margin < 0
            || p->position.y - margin > game.getWindowHeight()
            || p->position.x - margin > game.getWindowWidth())
        {
            delete p;
            // 把当前元素与末尾交换，再删末尾
            std::swap(*it, projectiles_enemy.back());
            projectiles_enemy.pop_back();
            SDL_Log("Enemy projectile removed");
            continue;
        }

        SDL_FRect rect2 = {p->position.x, p->position.y, p->width, p->height}; // 碰撞检测
        SDL_FRect rect = {player.position.x, player.position.y, player.width, player.height};
        if (SDL_HasRectIntersectionFloat(&rect, &rect2) && !is_dead)
        {
            // 碰撞处理
            player.current_hp -= p->damage;
            p->position.y = -1 - margin - p->height;
            p->speed = 0;
            MIX_PlayAudio(game.getMixer(), sounds["hit"]);
        }
        ++it;
    }
}

void SceneMain::render_projectiles_enemy()
{
    for (auto p : projectiles_enemy)
    {
        SDL_FRect rect = {p->position.x, p->position.y, p->width, p->height};
        float angle = std::atan2(p->direction.y, p->direction.x) * 180 / M_PI - 90;
        SDL_RenderTextureRotated(game.getRenderer(), p->texture, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
    }
}

void SceneMain::update_explosions(double)
{
    auto current_time = SDL_GetTicks();
    for (auto it = explosions.begin(); it != explosions.end();)
    {
        auto p = *it;
        p->current_frame = (current_time - p->start_time) * p->FPS / 1000.0f; // 更新帧数
        if (p->current_frame >= p->total_frames)
        {
            delete p;
            // 把当前元素与末尾交换，再删末尾
            std::swap(*it, explosions.back());
            explosions.pop_back();
            SDL_Log("Explosion removed");
            continue;
        }
        ++it;
    }
}

void SceneMain::render_explosions()
{
    for (auto p : explosions)
    {
        SDL_FRect src_rect = {p->current_frame * p->width / 2.0f, 0, p->width / 2.0f, p->height / 2.0f};
        SDL_FRect dst_rect = {p->position.x, p->position.y, p->width, p->height};
        SDL_RenderTexture(game.getRenderer(), p->texture, &src_rect, &dst_rect);
    }
}

SDL_FPoint SceneMain::get_direction(Enemy *enemy)
{
    SDL_FPoint direction;
    auto x = (player.position.x + player.width / 2.0f) - (enemy->position.x + enemy->width / 2.0f);
    auto y = (player.position.y + player.height / 2.0f) - (enemy->position.y + enemy->height / 2.0f);
    auto length = std::sqrt(x * x + y * y);
    if (length == 0)
    {
        direction.x = 0;
        direction.y = -1;
    }
    else
    {
        direction.x = x / length;
        direction.y = y / length;
    }
    return direction;
}
