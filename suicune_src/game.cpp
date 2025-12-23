#include "game.hpp"
#include "scene.hpp"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

void set_cwd_to_app_resources_if_present()
{
    const char *exe_dir = GetApplicationDirectory(); // .../Contents/MacOS when bundled

    if (!exe_dir)
        return;

    // If we're in a macOS .app, this folder should exist:
    //   <App>.app/Contents/Resources
    const char *resources_dir = TextFormat("%s/../Resources", exe_dir);

    if (DirectoryExists(resources_dir))
    {
        ChangeDirectory(exe_dir);
        ChangeDirectory("../Resources");
        TraceLog(LOG_INFO, "Changed CWD to bundle Resources: %s", GetWorkingDirectory());
    }
    else
    {
        TraceLog(LOG_INFO, "Not a bundle run; keeping CWD: %s", GetWorkingDirectory());
    }
}

namespace suicune
{
    Game::Game(const std::string &title, int window_width, int window_height, int tile_size)
        : title(title), window_width(window_width), window_height(window_height), tile_size(tile_size)
    {
        InitWindow(window_width, window_height, title.c_str());
        set_cwd_to_app_resources_if_present();

        running = true;

        SetTargetFPS(targetFps);
    }

    Game::~Game()
    {
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
        targetFps = fps;
        SetTargetFPS(fps);
    }

    bool Game::is_running() const
    {
        return running && !WindowShouldClose();
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

            float dt = GetFrameTime();

            g->begin_frame();

            if (g->current_scene)
            {
                g->current_scene->update(dt);
                g->current_scene->draw();
            }

            if (g->pending_scene)
                g->current_scene = std::move(g->pending_scene);

            current_scene->cleanup_eoframe();
            g->end_frame();
        };

        emscripten_set_main_loop(frame, 0, 1);
#else
        while (is_running())
        {
            float dt = GetFrameTime();

            begin_frame();

            if (current_scene)
            {
                current_scene->update(dt);
                current_scene->draw();
            }

            if (pending_scene)
                current_scene = std::move(pending_scene);

            current_scene->cleanup_eoframe();
            end_frame();
        }

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
    }

    void Game::end_frame()
    {
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
}
