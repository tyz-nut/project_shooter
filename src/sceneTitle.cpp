#include "sceneTitle.h"
#include "sceneMain.h"
#include "game.h"

SceneTitle::SceneTitle()
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::init()
{
    music_data = MIX_LoadAudio(game.getMixer(), "assets/music/06_Battle_in_Space_Intro.ogg", false);
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

void SceneTitle::update(double delta_time)
{
    timer += delta_time;
    if (timer >= 1.0f)
        timer = 0.0f;
}

void SceneTitle::render()
{
    std::string title = "SDL太空战机";
    game.render_text_center(title, 0.4f, true);

    if (timer < 0.5f)
    {
        std::string subtitle = "按 J 键开始游戏";
        game.render_text_center(subtitle, 0.8f, false);
    }
}

void SceneTitle::clean()
{
    MIX_StopTrack(game.getMusicTrack(), 0);
    MIX_SetTrackAudio(game.getMusicTrack(), nullptr);
    if (music_data != nullptr)
    {
        MIX_DestroyAudio(music_data);
    }
}

void SceneTitle::handleEvents(SDL_Event *event)
{
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.scancode == SDL_SCANCODE_J)
        {
            game.changeScene(new SceneMain());
        }
    }
}
