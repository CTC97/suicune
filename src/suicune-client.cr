require "socket"

require "../lib/sdl/src/sdl"
require "../lib/sdl/src/image"


class Client
    @suicune : SDL::Surface
    @nurse_joy : SDL::Surface

    def initialize(title : String, width : Int32, height : Int32)
        SDL.init(SDL::Init::VIDEO)
        at_exit { SDL.quit }

        SDL::IMG.init(SDL::IMG::Init::PNG)
        at_exit { SDL::IMG.quit }

        @client = TCPSocket.new("localhost", 1234)

        @window = SDL::Window.new(title, width, height)
        @renderer = SDL::Renderer.new(@window)

        @suicune = SDL::IMG.load("res/suicune.png")
        @suicune.alpha_mod = 88
        # @suicune = @suicune.convert(@window.surface)

        @running = true

        @suicune_x = 10
        @suicune_y = 10
        @suicune_speed = 5
        @bounds = 1

        # nurse joy
        @nurse_joy = SDL::IMG.load("res/nurse_joy.png")
        @nurse_joy_frames = StaticArray(SDL::Rect, 4).new do |i|
            SDL::Rect.new(i * 32, 0, 32, 42)
        end
        @nurse_joy_frame = 0
        @slowdown = 6
    end

    def run
        while @running
            while event = SDL::Event.poll
            case event
            when SDL::Event::Quit
                @running = false
            when SDL::Event::Keyboard
                case event.sym
                when .a?
                if @suicune_x - @suicune_speed - @bounds >= 0
                    @suicune_x -= @suicune_speed
                end
                when .d?
                if @suicune_x + @suicune.width + @suicune_speed + @bounds <= @window.width
                    @suicune_x += @suicune_speed
                end
                when .s?
                if @suicune_y + @suicune.height + @suicune_speed + @bounds <= @window.height
                    @suicune_y += @suicune_speed
                end
                when .w?
                if @suicune_y - @bounds - @suicune_speed >= 0
                    @suicune_y -= @suicune_speed
                end
                end
            end
            end

            clear_screen

            # suicune
            @renderer.copy(@suicune, nil, SDL::Rect[@suicune_x, @suicune_y, 64, 64])

            # nurse joy
            current_clip = @nurse_joy_frames[(@nurse_joy_frame / @slowdown).to_i]
            x = (@window.width - current_clip.w) / 2
            y = (@window.height - current_clip.h) / 2
            @renderer.copy(@nurse_joy, current_clip, SDL::Rect[x.to_i, y.to_i, current_clip.w, current_clip.h])

            @renderer.present

            @nurse_joy_frame = (@nurse_joy_frame + 1) % (@nurse_joy_frames.size * @slowdown)


            # psuedocode for game loop:

            # Frame rate management
            #current_time = SDL::get_ticks
            #frame_time = current_time - last_frame_time

            #if frame_delay > frame_time
            #SDL::delay(frame_delay - frame_time)
            #end

            #last_frame_time = SDL::get_ticks
            sleep(1/60)
        end
    end


    private def clear_screen
        @renderer.draw_color = SDL::Color[208, 236, 234, 255] # clear screen in white
        @renderer.clear
    end
end

client = Client.new("suicune v0.0.1", 640, 480)
client.run
