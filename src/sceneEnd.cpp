#include "sceneEnd.h"
#include "sceneMain.h"
#include "game.h"

SceneEnd::SceneEnd()
{
}

SceneEnd::~SceneEnd()
{
    clean();
}

void SceneEnd::init()
{
    if (!SDL_TextInputActive(game.getWindow()))
        SDL_StartTextInput(game.getWindow());
    if (!SDL_TextInputActive(game.getWindow()))
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_TextInputActive failed: %s", SDL_GetError());

    music_data = MIX_LoadAudio(game.getMixer(), "assets/music/06_Battle_in_Space_Intro.ogg", false);
    if (music_data == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "加载音乐失败: %s", SDL_GetError());
    }
    MIX_SetTrackAudio(game.getMusicTrack(), music_data);
    MIX_SetTrackGain(game.getMusicTrack(), 0.1f);
    SDL_PropertiesID play_props = SDL_CreateProperties();
    SDL_SetNumberProperty(play_props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    MIX_PlayTrack(game.getMusicTrack(), play_props);
    SDL_DestroyProperties(play_props);   
}

void SceneEnd::update(double delta_time)
{
    timer += delta_time;
    if (timer >= 1.0f)
    {
        timer -= 1.0f;
    }
}

void SceneEnd::render()
{
    if (is_type)
    {
        render_phase1();
    }
    else
    {
        render_phase2();
    }
}

void SceneEnd::clean()
{
    MIX_StopTrack(game.getMusicTrack(), 0);
    MIX_SetTrackAudio(game.getMusicTrack(), nullptr);
    if (music_data != nullptr)
    {
        MIX_DestroyAudio(music_data);
    }
}

void SceneEnd::handleEvents(SDL_Event *event)
{
    if (is_type)
    {
        if (event->type == SDL_EVENT_TEXT_INPUT)
        {
            name += event->text.text;
        }
        if (event->type == SDL_EVENT_KEY_DOWN)
        {
            if (event->key.scancode == SDL_SCANCODE_RETURN)
            {
                is_type = false;
                SDL_StopTextInput(game.getWindow());
                if (name.empty())
                {
                    name = "匿名";
                }
                game.insert_leaderboard(game.getFinalScore(), name);
            }
            if (event->key.scancode == SDL_SCANCODE_BACKSPACE)
            {
                if (!name.empty())
                {
                    remove_last_UTF8_char(name);
                }
            }
        }
    }
    else
    {
        if (event->type == SDL_EVENT_KEY_DOWN)
        {
            if (event->key.scancode == SDL_SCANCODE_J)
            {
                game.changeScene(new SceneMain());
            }
        }
    }
}

void SceneEnd::render_phase1()
{
    auto score = game.getFinalScore();
    std::string score_str = "你的得分是: " + std::to_string(score);
    std::string message = "GAME OVER";
    std::string message2 = "请输入你的名字，按回车键确认: ";

    game.render_text_center(score_str, 0.2f, false);
    game.render_text_center(message, 0.4f, true);
    game.render_text_center(message2, 0.6f, false);

    if (!name.empty())
    {
        SDL_FRect rect = game.render_text_center(name, 0.8f, false);
        if (timer > 0.5f)
            game.render_text_pos("_", rect.x + rect.w, rect.y);
    }
    else
    {
        if (timer > 0.5f)
            game.render_text_center("_", 0.8f, false);
    }
}

void SceneEnd::render_phase2()
{
    game.render_text_center("得分榜", 0.08f, true);
    int i = 1;
    for (const auto &pair : game.getLeaderboard())
    {
        game.render_text_pos(std::to_string(i) + ". " + pair.second, 100, 120 + i * 45, true);
        game.render_text_pos(std::to_string(pair.first), 100, 120 + i * 45, false);
        i++;
    }
    if (timer > 0.5f)
        game.render_text_center("按 J 键重新开始游戏", 0.85f, false);
}

void SceneEnd::remove_last_UTF8_char(std::string &str)
{
    auto last_char = str.back();
    if ((last_char & 0b10000000) == 0b10000000)
    {
        str.pop_back();
        while ((str.back() & 0b11000000) != 0b11000000)
        {
            str.pop_back();
        }
    }
    str.pop_back();
}
