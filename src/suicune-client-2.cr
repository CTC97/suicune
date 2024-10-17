require "socket"
require "json"

require "../lib/sdl/src/sdl"
require "../lib/sdl/src/image"

#const currentTime = Date.now();
#const timeElapsed = currentTime - GameRuntime.gameState.engine1.timePreviousLoop;
#const newLag = GameRuntime.gameState.engine1.lag + timeElapsed;
#GameRuntime.gameState.engine1.timePreviousLoop = currentTime;
#GameRuntime.gameState.engine1.lag = newLag;

class Client
    @suicune : SDL::Surface
    @nurse_joy : SDL::Surface
    @old_man : SDL::Surface

    # manage collisions with tiles via layers on a tilemap
    # user can assign different layers to assign with groups of entities
    @tilemap_resource : SDL::Surface
    @tilemap : Array(Array(Int32))


    @currentTime : Time::Span
    @lastTime : Time::Span

    @fps : Int32
    @experiencedFps : Float32

    def initialize(title : String, width : Int32, height : Int32, fps : Int32)
        @fps = fps
        @experiencedFps = fps.to_f32

        @tilemap = @tilemap = [
            [0, 5, 1, 0, 0, 0, 5, 0, 0, 5, 2, 0, 0, 4, 0, 3, 0, 0, 2, 0],
            [0, 0, 0, 0, 0, 0, 4, 4, 1, 3, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0],
            [0, 5, 0, 0, 2, 0, 1, 0, 3, 0, 3, 0, 4, 0, 2, 2, 4, 0, 0, 0],
            [5, 0, 0, 0, 0, 0, 0, 2, 1, 5, 0, 0, 1, 5, 2, 0, 0, 4, 0, 0],
            [0, 0, 0, 5, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 1, 3, 0, 0, 0, 0],
            [0, 1, 3, 0, 0, 0, 3, 0, 0, 0, 0, 5, 0, 0, 4, 0, 1, 0, 0, 0],
            [0, 1, 0, 3, 0, 1, 0, 0, 0, 0, 0, 0, 2, 5, 0, 0, 0, 4, 2, 0],
            [0, 5, 0, 2, 0, 0, 5, 5, 0, 1, 0, 0, 0, 0, 1, 0, 5, 0, 0, 0],
            [0, 0, 1, 0, 0, 0, 4, 5, 0, 0, 1, 0, 0, 5, 0, 0, 0, 0, 0, 3],
            [0, 3, 3, 0, 0, 4, 5, 2, 0, 0, 0, 0, 1, 2, 5, 0, 0, 0, 5, 4],
            [5, 0, 4, 0, 4, 4, 1, 4, 0, 0, 0, 3, 0, 0, 0, 0, 0, 5, 1, 5],
            [0, 4, 0, 0, 1, 0, 0, 5, 0, 0, 0, 5, 1, 1, 0, 0, 0, 3, 5, 0],
            [0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 5, 0, 5, 5, 3, 5, 3, 2],
            [1, 5, 0, 0, 0, 5, 4, 0, 5, 0, 0, 0, 3, 0, 1, 0, 1, 2, 0, 4],
            [0, 0, 0, 0, 2, 1, 2, 0, 3, 0, 5, 4, 0, 5, 0, 5, 5, 5, 2, 0],
            [0, 0, 0, 5, 0, 5, 1, 0, 0, 0, 3, 1, 0, 5, 0, 1, 0, 0, 0, 5],
            [0, 5, 0, 4, 0, 0, 0, 0, 3, 0, 4, 5, 0, 4, 0, 5, 0, 0, 0, 4],
            [0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 2, 5, 2, 0, 0, 0, 0, 0, 3, 0],
            [0, 0, 0, 5, 2, 0, 0, 0, 0, 5, 4, 0, 0, 5, 0, 1, 0, 0, 0, 0]
        ]


        @currentTime = Time.monotonic
        @lastTime = Time.monotonic

        SDL.init(SDL::Init::VIDEO)
        at_exit { SDL.quit }

        SDL::IMG.init(SDL::IMG::Init::PNG)
        at_exit { SDL::IMG.quit }

        @client = TCPSocket.new("localhost", 1234)

        @window = SDL::Window.new(title, width, height)
        @renderer = SDL::Renderer.new(@window)

        # look into <https://github.com/schovi/baked_file_system> for embedding files in binary

        @suicune = SDL::IMG.load("res/suicune.png")
        @suicune.alpha_mod = 88
        # @suicune = @suicune.convert(@window.surface)

        @running = true

        @suicune_x = 10
        @suicune_y = 10
        @suicune_speed = 25
        @bounds = 1

        # nurse joy
        @nurse_joy = SDL::IMG.load("res/nurse_joy.png")
        #
        @nurse_joy_frames = StaticArray(SDL::Rect, 4).new do |i|
            # i * width, 0, width, height
            SDL::Rect.new(i * 32, 0, 32, 42)
        end
        @nurse_joy_frame = 0

        # old man
        @old_man = SDL::IMG.load("res/old_man.png")
        @old_man_frames = StaticArray(SDL::Rect, 4).new do |i|
            SDL::Rect.new(i * 32, 0, 32, 38)
        end
        @old_man_frame = 0
        @old_man_x = 32 * 3
        @old_man_y = 32 * 3

        # tilemap
        @tilemap_resource = SDL::IMG.load("res/tilemap.png")
        @tilemap_tiles = StaticArray(SDL::Rect, 6).new do |i|
            # params here represent - (i * w, 0, w, h)
            SDL::Rect.new(i * 32, 0, 32, 32)
        end

        @slowdown = 6
    end

    def run
        while @running
            @currentTime = Time.monotonic

            delta_time = (@currentTime - @lastTime).milliseconds

            #@experiencedFps = (@experiencedFps * 0.9) + ((1000.0 / delta_time) * 0.1)
            @experiencedFps = (1000 / delta_time).to_f32.round(2)


            if delta_time > 1000/@fps

                data_packet = ({
                    "old_man": {
                        x: @old_man_x,
                        y: @old_man_y,
                        frame: @old_man_frame
                    }
                }).to_json

                @client.puts data_packet
                server_response = @client.gets || ""
                server_data_packet = JSON.parse(server_response)

                old_man_x = server_data_packet["old_man"]["x"].as_i
                old_man_y = server_data_packet["old_man"]["y"].as_i
                puts "#{old_man_x}, #{old_man_y}"

                @window.title = "suicune 0.0.a [#{@experiencedFps} FPS]"
                @lastTime = @currentTime
                while event = SDL::Event.poll

                    case event
                    when SDL::Event::Quit
                        @running = false
                    when SDL::Event::Keyboard
                        #@window.title = "suicune 0.0.a < [#{@experiencedFps} FPS] | [#{event.type}] >"
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

                @tilemap.each_with_index do |row, i|
                    row.each_with_index do |value, j|
                        @renderer.copy(@tilemap_resource, @tilemap_tiles[value], SDL::Rect[i*32, j*32, 32, 32])
                    end
                end

                # suicune
                @renderer.copy(@suicune, nil, SDL::Rect[@suicune_x, @suicune_y, 64, 64])

                # nurse joy
                current_clip = @nurse_joy_frames[(@nurse_joy_frame / @slowdown).to_i]
                x = (@window.width - current_clip.w) / 2
                y = (@window.height - current_clip.h) / 2
                @renderer.copy(@nurse_joy, current_clip, SDL::Rect[x.to_i, y.to_i, current_clip.w, current_clip.h])

                # old man
               # old_man_frame = @old_man_frames[0]
                old_man_clip = @old_man_frames[(@old_man_frame / @slowdown).to_i]
                @renderer.copy(@old_man, old_man_clip, SDL::Rect[old_man_x, old_man_y, old_man_clip.w, old_man_clip.h])

                @renderer.present

                @old_man_frame = (@old_man_frame + 1) % (@old_man_frames.size * @slowdown)


                # psuedocode for game loop:

                # Frame rate management
                #current_time = SDL::get_ticks
                #frame_time = current_time - last_frame_time

                #if frame_delay > frame_time
                #SDL::delay(frame_delay - frame_time)
                #end

                #last_frame_time = SDL::get_ticks
                #sleep(1/60)
            end
        end
    end


    private def clear_screen
        @renderer.draw_color = SDL::Color[208, 236, 234, 255] # clear screen in white
        @renderer.clear
    end
end

client = Client.new("suicune v0.0.1", 640, 640, 60)
client.run
