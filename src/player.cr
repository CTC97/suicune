require "../lib/sdl/src/sdl"
require "../lib/sdl/src/image"
require "../lib/sdl/src/lib_sdl/keyboard"
require "../lib/sdl/src/lib_sdl/keycode"
require "./sprite.cr"
require "./library/pokedex.cr"

class Player

    @player_id : Int32

    @x : Float32
    @y : Float32
    @speed : Int32

    @dir : Int32

    @collision_map : Array(Array(Bool))

    @poke_sprite : Sprite

    # TODO - Add parameters to player
    #   number of frames
    #   sprite height, width
    #   resource location
    def initialize()
        @player_id = Random::DEFAULT.rand(Int32)
        @x = 0
        @y = 0
        @speed = 32

        @bounds = 0

        @dir = 0

        @collision_map = Array(Array(Bool)).new()

        @poke_sprite = Pokedex.pokemon_sprites.values.sample
        @poke_sprite.play_animation("up")
    end

    # need to pass in window width
    def update(event : SDL::Event?, window_width : Int32, window_height : Int32, delta_time : Float)
        update_speed = @speed

        tile_x = (@x/(@collision_tile_size || 1)).round.to_i
        tile_y = (@y/(@collision_tile_size || 1)).round.to_i
        

        case event
        when SDL::Event::Keyboard
            if event.type == LibSDL::EventType::KEYDOWN
                case event.sym
                    when .a?
                        intended_tile_x = tile_x - 1
                        # TODO - need to update the below bounds checks with MAP bounds, not window bounds

                        #@x - update_speed - @bounds >= 0 && 
                        if !check_collision(intended_tile_x, tile_y)
                            @x -= update_speed
                        end
                        @poke_sprite.play_animation("left")
                    when .d?
                        intended_tile_x = tile_x + 1
                        #@x + @poke_sprite.frames[0].w + update_speed + @bounds <= window_width &&
                        if !check_collision(intended_tile_x, tile_y)
                            @x += update_speed
                        end
                        @poke_sprite.play_animation("right")
                    when .s?
                        intended_tile_y = tile_y + 1
                        #@y + @poke_sprite.frames[0].h + update_speed + @bounds <= window_height && 
                        if !check_collision(tile_x, intended_tile_y) 
                            @y += update_speed
                        end
                        @poke_sprite.play_animation("down")
                    when .w?
                        intended_tile_y = tile_y - 1
                        #@y - @bounds - update_speed >= 0 && 
                        if !check_collision(tile_x, intended_tile_y)
                            @y -= update_speed
                        end
                        @poke_sprite.play_animation("up")
                    end
                end
           
        end

        @poke_sprite.frame += 1
    end

    def sprite
        @poke_sprite.surface
    end

    def set_collision_map(collision_map : Array(Array(Bool)), collision_tile_size : Int32)
        @collision_map = collision_map
        @collision_tile_size = collision_tile_size
    end

    def check_collision(x : Int32, y : Int32)
        @collision_map[y][x]
    end

    def player_id
        @player_id
    end

    def x
        @x.to_i
    end

    def y
        @y.to_i
    end

    def poke_sprite
        @poke_sprite
    end

    def current_frame 
        @poke_sprite.current_frame
    end

    def current_sprite_frame
        @poke_sprite.current_sprite_frame
    end

    def frame
        @poke_sprite.frame
    end

    def dir
        @dir
    end

end
