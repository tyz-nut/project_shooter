#include "game.h"
#include "sceneMain.h"
#include "sceneTitle.h"
#include <fstream>

Game::Game()
{
}

Game::~Game()
{
    save_data(); // 保存得分榜的数据
    clean(); // 清理资源
}

void Game::run()
{
    while (is_running)
    {
        auto start_time = SDL_GetTicks();

        SDL_Event event;
        handleEvents(&event);
        update();
        render();

        auto end_time = SDL_GetTicks();
        auto diff = end_time - start_time;
        if (diff < frame_time)
        {
            SDL_Delay(frame_time - diff);
            delta_time = frame_time / 1000.0;
        }
        else
        {
            delta_time = diff / 1000.0;
        }
    }
}

void Game::init()
{
    // SDL初始化
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s", SDL_GetError());
        is_running = false;
    }
    // 创建窗口
    window = SDL_CreateWindow("Shooter", window_width, window_height, 0);
    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow failed: %s", SDL_GetError());
        is_running = false;
    }

    // 创建渲染器
    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateRenderer failed: %s", SDL_GetError());
        is_running = false;
    }

    // 设置逻辑分辨率
    SDL_SetRenderLogicalPresentation(renderer, window_width, window_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    
    // 初始化播放器
    if (!MIX_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "MIX_Init failed: %s", SDL_GetError());
        is_running = false;
    }
    // 创建mixer
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    // 创建音轨
    music_track = MIX_CreateTrack(mixer);

    // 初始化字体
    if (!TTF_Init())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_Init failed: %s", SDL_GetError());
        is_running = false;
    }

    // 创建场景
    // current_scene = new SceneMain();
    current_scene = new SceneTitle();
    current_scene->init(); // 初始化场景

    // 加载背景卷轴
    near_star.texture = IMG_LoadTexture(renderer, "assets/image/Stars-A.png");
    if (near_star.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_LoadTexture failed: %s", SDL_GetError());
        is_running = false;
    }
    SDL_GetTextureSize(near_star.texture, &near_star.width, &near_star.height);
    near_star.height /= 2;
    near_star.width /= 2;
    near_star.speed = 40;

    far_star.texture = IMG_LoadTexture(renderer, "assets/image/Stars-B.png");
    if (far_star.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "IMG_LoadTexture failed: %s", SDL_GetError());
        is_running = false;
    }
    SDL_GetTextureSize(far_star.texture, &far_star.width, &far_star.height);
    far_star.height /= 2;
    far_star.width /= 2;
    far_star.speed = 20;

    // 加载字体
    title_font = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 64);
    text_font = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 32);
    if (title_font == nullptr || text_font == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_OpenFont failed: %s", SDL_GetError());
        is_running = false;
    }

    // 载入得分榜
    load_data();
}

void Game::clean()
{
    if (title_font != nullptr)
    {
        TTF_CloseFont(title_font);
    }
    if (text_font != nullptr)
    {
        TTF_CloseFont(text_font);
    }
    if (near_star.texture != nullptr)
    {
        SDL_DestroyTexture(near_star.texture);
    }
    if (far_star.texture != nullptr)
    {
        SDL_DestroyTexture(far_star.texture);
    }

    if (current_scene != nullptr)
    {
        current_scene->clean();
        delete current_scene;
    }
    TTF_Quit();
    MIX_DestroyMixer(mixer); // 销毁mixer时自动销毁track
    MIX_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::changeScene(Scene *scene)
{
    if (current_scene != nullptr)
    {
        current_scene->clean();
        delete current_scene;
    }
    current_scene = scene;
    current_scene->init();
}

void Game::handleEvents(SDL_Event* event)
{
    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_EVENT_QUIT)
        {
            is_running = false;
            break;
        }
        if (event->type == SDL_EVENT_KEY_DOWN)
        {
            if (event->key.scancode == SDL_SCANCODE_F4)
            {
                is_fullscreen = !is_fullscreen; // 切换全屏模式
                if (is_fullscreen)
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                else
                    SDL_SetWindowFullscreen(window, 0);
            }
        }
        current_scene->handleEvents(event);
    }
}

void Game::update()
{
    update_background();
    current_scene->update(delta_time);
}

void Game::render()
{
    SDL_RenderClear(renderer);
    render_background();
    current_scene->render();
    SDL_RenderPresent(renderer);
}

SDL_FRect Game::render_text_center(const std::string &text, float pos_y, bool is_title)
{
    SDL_Color color = {255, 255, 255, 255};
    auto font = is_title ? title_font : text_font;
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), 0, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(getRenderer(), surface);
    SDL_FRect rect = {static_cast<float>(getWindowWidth() / 2.0f - surface->w / 2.0f), 
                        static_cast<float>((getWindowHeight() - surface->h) * pos_y), 
                        static_cast<float>(surface->w), 
                        static_cast<float>(surface->h)};
    SDL_RenderTexture(getRenderer(), texture, nullptr, &rect);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
    return rect;
}

SDL_FRect Game::render_text_pos(const std::string &text, float pos_x, float pos_y, bool is_left)
{
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(text_font, text.c_str(), 0, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(getRenderer(), surface);
    float x = is_left ? pos_x : getWindowWidth() -  pos_x - surface->w;
    SDL_FRect rect = {x, pos_y, static_cast<float>(surface->w), static_cast<float>(surface->h)};
    SDL_RenderTexture(getRenderer(), texture, nullptr, &rect);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
    return rect;
}

void Game::insert_leaderboard(int score, const std::string &name)
{
    leaderboard.insert({ score, name });
    if (leaderboard.size() > 10)
    {
        leaderboard.erase(--leaderboard.end());
    }
}

void Game::update_background()
{
    near_star.offset += near_star.speed * delta_time;
    if (near_star.offset > 0)
    {
        near_star.offset -= near_star.height;
    }

    far_star.offset += far_star.speed * delta_time;
    if (far_star.offset > 0)
    {
        far_star.offset -= far_star.height;
    }
}

void Game::render_background()
{
    for (float pos_y = far_star.offset; pos_y < getWindowHeight(); pos_y += far_star.height)
    {
        for (float pos_x = 0; pos_x < getWindowWidth(); pos_x += far_star.width)
        {
            SDL_FRect dst_rect = { pos_x, pos_y, far_star.width, far_star.height };
            SDL_RenderTexture(getRenderer(), far_star.texture, nullptr, &dst_rect);
        }
    }
    for (float pos_y = near_star.offset; pos_y < getWindowHeight(); pos_y += near_star.height)
    {
        for (float pos_x = 0; pos_x < getWindowWidth(); pos_x += near_star.width)
        {
            SDL_FRect dst_rect = { pos_x, pos_y, near_star.width, near_star.height };
            SDL_RenderTexture(getRenderer(), near_star.texture, nullptr, &dst_rect);
        }
    }
}

void Game::save_data()
{
    // 保存得分榜的数据
    std::ofstream file("assets/save.dat");
    if (!file.is_open())
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to open file: assets/save.dat");
        return;
    }
    for (const auto& entry : leaderboard)
    {
        file << entry.first << " " << entry.second << std::endl;
    }
    file.close();
}

void Game::load_data()
{
    // 读取得分榜的数据
    std::ifstream file("assets/save.dat");
    if (!file.is_open())
    {
        SDL_Log("Failed to open file: assets/save.dat");
        return;
    }
    leaderboard.clear();
    int score;
    std::string name;
    while (file >> score >> name)
    {
        leaderboard.insert({ score, name });
    }
    file.close();
}
