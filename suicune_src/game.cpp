#include "game.hpp"
#include "scene.hpp"
#include "util.hpp"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

namespace suicune
{
    Game::Game(const std::string &title, int window_width, int window_height, int tile_size)
        : title(title), window_width(window_width), window_height(window_height), tile_size(tile_size)
    {
        InitWindow(window_width, window_height, title.c_str());
        set_cwd_to_app_resources_if_present();

        running = true;

        SetTargetFPS(target_fps);
    }

    Game::~Game()
    {
        global_shader.reset();

        if (IsWindowReady())
            CloseWindow();
    }

    void Game::set_fullscreen(bool enabled)
    {
        fullscreen = enabled;

        if (enabled)
        {
            if (!IsWindowFullscreen())
            {
                ToggleFullscreen();
            }
        }
        else
        {
            if (IsWindowFullscreen())
            {
                ToggleFullscreen();
            }
        }
    }

    void Game::set_resizable(bool enabled)
    {
        resizable = enabled;

        if (enabled)
        {
            SetWindowState(FLAG_WINDOW_RESIZABLE);
        }
        else
        {
            ClearWindowState(FLAG_WINDOW_RESIZABLE);
        }
    }

    void Game::set_fps(int fps)
    {
        target_fps = fps;
        SetTargetFPS(fps);
    }

    bool Game::is_running() const
    {
        return running && !WindowShouldClose();
    }

    void Game::tick(float dt)
    {
        begin_frame();

        if (current_scene)
        {
            current_scene->update(dt);
            current_scene->draw();
        }

        if (pending_scene)
            current_scene = std::move(pending_scene);

        if (current_scene)
            current_scene->cleanup_eoframe();

        end_frame();
    }

    void Game::run()
    {
#if defined(PLATFORM_WEB)
        // Web: the browser owns the loop. We register a callback instead of blocking.
        static Game *g = nullptr;
        g = this;

        auto frame = []()
        {
            if (!g)
                return;

            // allow quit()
            if (!g->running)
            {
                emscripten_cancel_main_loop();
                return;
            }

            g->tick(GetFrameTime());
        };

        emscripten_set_main_loop(frame, 0, 1);
#else
        while (is_running())
            tick(GetFrameTime());

        running = false;
#endif
    }

    void Game::quit()
    {
        running = false;
    }

    void Game::begin_frame()
    {

        BeginDrawing();
        ClearBackground(BLACK);

        if (global_shader)
            begin_timed_shader_mode(*global_shader, (float)get_window_width(), (float)get_window_height());
        Color custom_color = {180, 203, 240, 255};
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), custom_color);
    }

    void Game::end_frame()
    {
        if (global_shader)
        {
            EndShaderMode();
        }
        EndDrawing();
    }

    int Game::get_window_width() const
    {
        return window_width;
    }

    int Game::get_window_height() const
    {
        return window_height;
    }

    int Game::get_tile_size() const
    {
        return tile_size;
    }

    void Game::request_scene(std::unique_ptr<Scene> scene)
    {
        if (!scene)
            return;

        pending_scene = std::move(scene);
    }

    DialogManager &Game::get_dialog_manager()
    {
        return dialog_manager;
    }

    void Game::set_dev_mode(bool enabled)
    {
        dev_mode = enabled;
    }

    bool Game::is_dev_mode() const
    {
        return dev_mode;
    }

    void Game::set_global_shader(std::shared_ptr<Shader> shader)
    {
        global_shader = std::move(shader);
    }

    void Game::clear_global_shader()
    {
        global_shader.reset();
    }

    DIRECTIONAL_MODE Game::get_direction_mode() const
    {
        return direction_mode;
    }

    void Game::set_direction_mode(DIRECTIONAL_MODE mode)
    {
        direction_mode = mode;
    }
}
