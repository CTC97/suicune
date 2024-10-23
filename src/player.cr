require "../lib/sdl/src/sdl"
require "../lib/sdl/src/image"
require "../lib/sdl/src/lib_sdl/keyboard"
require "../lib/sdl/src/lib_sdl/keycode"

class Player

    @player_id : Int32

    @x : Float32
    @y : Float32
    @speed : Int32

    @sprite : SDL::Surface
    @frame : Int32
    @dir : Int32

    @slowdown = 6

    @collision_map : Array(Array(Bool))

    #@animation_dict : Hash(String, Array(SDL::Rect))
    @animation_dict : Hash(String, Array(Int32))
    #@current_animation : Array(SDL::Rect)
    @current_animation : Array(Int32)
    @animation_speed : Float64

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

        @sprite = SDL::IMG.load("res/togepi.png")

        # 12 is number of frames
        @frames = Array(SDL::Rect).new(16) do |i|
            SDL::Rect.new(i * 32, 0, 32, 32)
        end
        #@animation_dict = {} of String => Array(SDL::Rect)
        @animation_dict = {} of String => Array(Int32)
        #@current_animation = [] of SDL::Rect
        @current_animation = [] of Int32

        @frame = 0
        @dir = 0

        @collision_map = Array(Array(Bool)).new()

        define_animation(@frames, [4,5,6,7], "left")
        define_animation(@frames, [8,9,10,11], "right")
        define_animation(@frames, [0,1,2,3], "down")
        define_animation(@frames, [12,13,14,15], "up")
        @animation_speed = 0.25;
        play_animation("up")
    end

    def define_animation(frames : Array(SDL::Rect), animation_frames : Array(Int32), name : String)
        #@animation_dict[name] = (animation_frames.map { |frame| frames[frame] })
        @animation_dict[name] = animation_frames
    end

    def play_animation(name : String)
        @current_animation = @animation_dict[name]
    end

    # need to pass in window width
    def update(event : SDL::Event?, window_width : Int32, window_height : Int32, delta_time : Float)
        #puts "delta_time: #{delta_time}"
        update_speed = @speed
        #puts "update_speed: #{update_speed}"
        
        # keys = LibSDL.get_keyboard_state(nil)
        # SDL_Scancodes - https://wiki.libsdl.org/SDL3/SDL_Scancode
        # A - 4
        # D - 7
        # S - 22
        # W - 26

        # puts keys[4]

        # A
        #if keys[4] == 1
        #    if @x - update_speed - @bounds >= 0
        #        @x -= update_speed
        #    end
        #    @dir = 2
        #end
        # 
        tile_x = (@x/(@collision_tile_size || 1)).round.to_i
        tile_y = (@y/(@collision_tile_size || 1)).round.to_i
        

        case event
        when SDL::Event::Keyboard
            case event.sym
            when .a?
                intended_tile_x = tile_x - 1
                if @x - update_speed - @bounds >= 0 && !check_collision(intended_tile_x, tile_y)
                    @x -= update_speed
                end
                play_animation("left")
            when .d?
                intended_tile_x = tile_x + 1
                if @x + @frames[0].w + update_speed + @bounds <= window_width && !check_collision(intended_tile_x, tile_y)
                    @x += update_speed
                end
                play_animation("right")
            when .s?
                intended_tile_y = tile_y + 1
                if @y + @frames[0].h + update_speed + @bounds <= window_height && !check_collision(tile_x, intended_tile_y) 
                    @y += update_speed
                end
                play_animation("down")
            when .w?
                intended_tile_y = tile_y - 1
                if @y - @bounds - update_speed >= 0 && !check_collision(tile_x, intended_tile_y)
                    @y -= update_speed
                end
                play_animation("up")
            end
        end

       @frame += 1
       # @current_animation
    end

    def set_collision_map(collision_map : Array(Array(Bool)), collision_tile_size : Int32)
        @collision_map = collision_map
        @collision_tile_size = collision_tile_size
    end

    def check_collision(x : Int32, y : Int32)
        @collision_map[x][y]
    end

    def current_sprite_frame
        #@current_animation[(@frame * @animation_speed).to_i % @current_animation.size]
        @frames[@current_animation[(@frame * @animation_speed).to_i % @current_animation.size]]
    end

    # used for the frame in the sheet, NOT the frame in the animation
    def current_frame
        @current_animation[(@frame * @animation_speed).to_i % @current_animation.size]
    end

    def sprite
        @sprite
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

    def frame
        @frame
    end

    def dir
        @dir
    end

end
