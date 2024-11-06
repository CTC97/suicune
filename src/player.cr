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
    @original_speed : Int32
    @running : Bool

    @tile_x : Int32
    @tile_y : Int32

    @dir : Int32

    @collision_map : Array(Array(Bool))

    @poke_sprite : Sprite

    @scale : Float64

    # TODO - Add parameters to player
    #   number of frames
    #   sprite height, width
    #   resource location
    def initialize()
        @player_id = Random::DEFAULT.rand(Int32)

        @scale = 2

        @x = 0
        @y = 0
        @tile_x = 0
        @tile_y = 0

        @speed = @original_speed = 8
        @running = false

        @bounds = 0

        @dir = 0

        @collision_map = Array(Array(Bool)).new()

        @poke_sprite = Pokedex.pokemon_sprites.values.sample
        @poke_sprite.play_animation("up")
    end

    # need to pass in window width
    def update(event : SDL::Event?, window_width : Int32, window_height : Int32, delta_time : Float)
        update_speed = @speed

        @tile_x = (@x/(@collision_tile_size || 1)).to_i
        @tile_y = (@y/(@collision_tile_size || 1)).to_i
        

        case event
        when SDL::Event::Keyboard
            if event.type == LibSDL::EventType::KEYDOWN
                case event.sym
                    when .lshift?
                        @running = !@running
                        @speed = @running ? @original_speed * 2 : @original_speed
                        @poke_sprite.animation_speed = @running ? @poke_sprite.original_animation_speed * 2 : @poke_sprite.original_animation_speed
                    when .equals?
                        @scale += 0.5 if scale < 4
                    when .minus?
                        @scale -= 0.5 if scale > 1
                    when .a?
                        intended_tile_x = tile_x - 1
                        # TODO - need to update the below bounds checks with MAP bounds, not window bounds

                        #@x - update_speed - @bounds >= 0 && 
                        if !(check_collision(intended_tile_x.ceil.to_i, tile_y.floor.to_i) || check_collision(intended_tile_x.ceil.to_i, tile_y.ceil.to_i))
                            @x -= update_speed
                        end
                        @poke_sprite.play_animation("left")
                    when .d?
                        intended_tile_x = tile_x + 1
                        #@x + @poke_sprite.frames[0].w + update_speed + @bounds <= window_width &&
                        if !(check_collision(intended_tile_x.floor.to_i, tile_y.floor.to_i) || check_collision(intended_tile_x.floor.to_i, tile_y.ceil.to_i))
                            @x += update_speed
                        end
                        @poke_sprite.play_animation("right")
                    when .s?
                        intended_tile_y = tile_y + 1
                        #@y + @poke_sprite.frames[0].h + update_speed + @bounds <= window_height && 
                        if !(check_collision(tile_x.floor.to_i, intended_tile_y.floor.to_i) || check_collision(tile_x.ceil.to_i, intended_tile_y.floor.to_i))
                            @y += update_speed
                        end
                        @poke_sprite.play_animation("down")
                    when .w?
                        intended_tile_y = tile_y - 1
                        #@y - @bounds - update_speed >= 0 && 
                        if !(check_collision(tile_x.floor.to_i, intended_tile_y.ceil.to_i) || check_collision(tile_x.ceil.to_i, intended_tile_y.ceil.to_i))
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
        return true if (y < 0 || y >= @collision_map.size ) || (x < 0 || x >= @collision_map[y].size)
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

    def tile_x 
        @tile_x
    end

    def tile_y
        @tile_y
    end

    def scale 
        @scale
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
