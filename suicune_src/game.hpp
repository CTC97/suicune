#pragma once

#include <memory>
#include <string>
#include "raylib.h"
#include "dialog_manager.hpp"

namespace suicune
{

    class Scene;

    class Game
    {
    public:
        Game(const std::string &title, int window_width, int window_height, int tile_size);
        ~Game();

        void run();
        bool is_running() const;
        void quit();

        void set_fullscreen(bool enabled);
        void set_resizable(bool enabled);
        void set_fps(int fps);

        int get_window_width() const;
        int get_window_height() const;

        int get_tile_size() const;

        void request_scene(std::unique_ptr<Scene> scene);

        DialogManager &get_dialog_manager();

        void set_dev_mode(bool enabled);
        bool is_dev_mode() const;

        void set_global_shader(std::shared_ptr<Shader> shader);
        void clear_global_shader();

    private:
        std::string title;
        int window_width;
        int window_height;

        int tile_size;

        bool fullscreen = false;
        bool resizable = false;
        int target_fps = 60;

        bool running = false;

        std::unique_ptr<Scene> current_scene;
        std::unique_ptr<Scene> pending_scene;

        DialogManager dialog_manager;

        void begin_frame();
        void end_frame();

        bool dev_mode = false;

        std::shared_ptr<Shader> global_shader;
    };

}
