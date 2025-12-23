#pragma once

#include "raylib.h"
#include "spritesheet.hpp"
#include "util.hpp"

namespace suicune
{

    class Game;

    class Scene
    {
    public:
        explicit Scene(Game &game);
        virtual ~Scene();

        virtual void update(float dt);
        virtual void draw();

        virtual void cleanup_eoframe() = 0;

        void request_scene(std::unique_ptr<Scene> new_scene);

        Game &get_game();

        Camera2D &get_camera();

    protected:
        Game &game;
        Camera2D camera;

        bool transitioning_scene = false;

        virtual std::shared_ptr<Spritesheet> define_spritesheet(const char *file_path, int frame_width, int frame_height);

        Shake screen_shake;
        void shake(float strength, float duration);

        virtual std::shared_ptr<Shader> define_shader(const char *file_path);

        std::shared_ptr<Shader> scene_shader;
        void set_scene_shader(std::shared_ptr<Shader> shader);
        void clear_scene_shader();

    private:
        std::unordered_map<std::string, std::shared_ptr<Spritesheet>> spritesheets;
        void unload_spritesheets();

        std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
        void unload_shaders();
    };

}
