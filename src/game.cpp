#include "game.hpp"
#include "scene.hpp"

namespace barley
{
    Game::Game(const std::string &title, int window_width, int window_height, int tile_size)
        : title(title), window_width(window_width), window_height(window_height), tile_size(tile_size)
    {
        InitWindow(window_width, window_height, title.c_str());
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
        while (is_running())
        {
            float dt = GetFrameTime();

            begin_frame();

            if (current_scene)
            {
                current_scene->update(dt);
                current_scene->draw();
            }

            end_frame();
        }

        running = false;
    }

    void Game::quit()
    {
        running = false;
        CloseWindow();
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

    void Game::set_scene(std::unique_ptr<Scene> scene)
    {
        current_scene = std::move(scene);
    }
}
