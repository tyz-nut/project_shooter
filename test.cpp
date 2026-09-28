#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

int main(int, char **)
{
    std::cout << "hello world" << std::endl;

    // SDL3初始化
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("Hello World!", 800, 600, 0);
    // 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    // SDL3_image不需要手动初始化
    // 加载图片
    SDL_Texture* texture = IMG_LoadTexture(renderer, "assets/image/bg.png");

    // SDL_Mixer初始化
    if (!MIX_Init())
    {
        std::cerr << "MIX_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // 创建mixer
    MIX_Mixer* mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    // 创建音轨
    MIX_Track* music_track = MIX_CreateTrack(mixer);
    // 加载音乐
    MIX_Audio* music_data = MIX_LoadAudio(mixer, "assets/music/03_Racing_Through_Asteroids_Loop.ogg", false);
    // 把数据放到音轨上
    MIX_SetTrackAudio(music_track, music_data);
    // 播放音乐(单次循环)
    // MIX_PlayTrack(music_track, 0);

    // 播放音乐(无限循环)
    SDL_PropertiesID play_props = SDL_CreateProperties();               // 创造一个空的属性容器
    SDL_SetNumberProperty(play_props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);  // 设置循环次数为-1，表示无限循环
    MIX_PlayTrack(music_track, play_props); // 播放音轨，传入属性容器
    SDL_DestroyProperties(play_props); // 播放后可以销属性容器，Mixer会复制需要的数据

    //读取音效数据
    MIX_Audio *effect_data = MIX_LoadAudio(mixer, "assets/music/laser.wav", true);

    // SDL_TTF初始化
    if (!TTF_Init())
    {
        std::cerr << "TTF_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // 加载字体
    TTF_Font* font = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 24);
    // 创建文本纹理
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Hello World!", 0, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // 渲染循环
    bool running = true;
    while (running)
    {
        // 处理事件
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                MIX_PlayAudio(mixer, effect_data);
            }
        }

        // 清屏
        SDL_RenderClear(renderer);

        // 画一个长方形
        SDL_FRect rect = {100, 100, 200, 200};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        // 画图片
        SDL_FRect dstRect = {0, 0, 800, 600};
        SDL_RenderTexture(renderer, texture, NULL, &dstRect);

        // 画文本
        SDL_FRect textRect = {100, 300, 200, 50};
        SDL_RenderTexture(renderer, textTexture, NULL, &textRect);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        // 更新屏幕
        SDL_RenderPresent(renderer);
    }

    // 清理资源
    SDL_DestroyTexture(texture);
    
    MIX_DestroyAudio(music_data);
    MIX_DestroyAudio(effect_data);
    MIX_DestroyMixer(mixer); // 销毁mixer时自动销毁track
    MIX_Quit();
    
    SDL_DestroySurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}