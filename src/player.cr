require "../lib/sdl/src/sdl"
require "../lib/sdl/src/image"

class Player

    @player_id : Int32

    @x : Int32
    @y : Int32
    @speed : Int32

    @sprite : SDL::Surface
    @frame : Int32
    @dir : Int32

    @slowdown = 6

    def initialize()
        @player_id = Random::DEFAULT.rand(Int32)
        @x = 10
        @y = 10
        @speed = 10

        @bounds = 1

        @sprite = SDL::IMG.load("res/nurse_joy.png")

        @frames = StaticArray(SDL::Rect, 4).new do |i|
            # i * width, 0, width, height
            SDL::Rect.new(i * 32, 0, 32, 42)
        end

        @frame = 0
        @dir = 0
    end

    # need to pass in window width
    def update(event : SDL::Event, window_width : Int32, window_height : Int32)
        case event
        when SDL::Event::Keyboard
            case event.sym
            when .a?
                if @x - @speed - @bounds >= 0
                    @x -= @speed
                end
                @dir = 2
            when .d?
                if @x + @sprite.width + @speed + @bounds <= window_width
                    @x += @speed
                end
                @dir = 0
            when .s?
                if @y + @sprite.height + @speed + @bounds <= window_height
                    @y += @speed
                end
                @dir = 3
            when .w?
                if @y - @bounds - @speed >= 0
                    @y -= @speed
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
        @x
    end

    def y
        @y
    end

    def frame
        @frame
    end

    def dir
        @dir
    end

end
