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

    def initialize()
        @player_id = Random::DEFAULT.rand(Int32)
        @x = 0
        @y = 0
        @speed = 16

        @bounds = 0

        @sprite = SDL::IMG.load("res/nurse_joy.png")

        @frames = StaticArray(SDL::Rect, 4).new do |i|
            # i * width, 0, width, height
            SDL::Rect.new(i * 32, 0, 32, 42)
        end

        @frame = 0
        @dir = 0
    end

    # need to pass in window width
    def update(event : SDL::Event, window_width : Int32, window_height : Int32, delta_time : Float)
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

        case event
        when SDL::Event::Keyboard
            case event.sym
            when .a?
                if @x - update_speed - @bounds >= 0
                    @x -= update_speed
                end
                @dir = 2
            when .d?
                puts (@x)
                puts (@x + @sprite.width)
                if @x + @frames[0].w + update_speed + @bounds <= window_width
                    @x += update_speed
                end
                @dir = 0
            when .s?
                if @y + @frames[0].h + update_speed + @bounds <= window_height
                    @y += update_speed
                end
                @dir = 3
            when .w?
                if @y - @bounds - update_speed >= 0
                    @y -= update_speed
                end
                @dir = 1
            end
        end


        @frame = (@frame + 1) % (@frames.size * @slowdown)
    end

    def current_sprite_frame
        @frames[@dir]
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
