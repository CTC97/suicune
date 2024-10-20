require "socket"
require "json"

require "../lib/sdl/src/sdl"
require "../lib/sdl/src/image"

require "./player"
require "./tilemap"

#const currentTime = Date.now();
#const timeElapsed = currentTime - GameRuntime.gameState.engine1.timePreviousLoop;
#const newLag = GameRuntime.gameState.engine1.lag + timeElapsed;
#GameRuntime.gameState.engine1.timePreviousLoop = currentTime;
#GameRuntime.gameState.engine1.lag = newLag;

class Client
    @suicune : SDL::Surface
    @other_player : SDL::Surface

    @player : Player

    @global_player_stats : Hash(String, Hash(String, Int32))

    @tilemap : Tilemap

    @global_player_stats = Hash(String, Hash(String, Int32)).new

    @currentTime : Time::Span
    @lastTime : Time::Span

    @fps : Int32
    @experiencedFps : Float32

    @camera : SDL::Rect

    def initialize(title : String, width : Int32, height : Int32, fps : Int32)
        @fps = fps
        @experiencedFps = fps.to_f32

        @currentTime = Time.monotonic
        @lastTime = Time.monotonic

        SDL.init(SDL::Init::VIDEO)
        at_exit { SDL.quit }

        SDL::IMG.init(SDL::IMG::Init::PNG)
        at_exit { SDL::IMG.quit }

        @client = TCPSocket.new("localhost", 1234)

        @player = Player.new

        # used for other players
        @other_player = SDL::IMG.load("res/togepi.png")
        @other_player_frames = StaticArray(SDL::Rect, 16).new do |i|
            # i * width, 0, width, height
            SDL::Rect.new(i * 32, 0, 32, 32)
        end
        @other_player_frame = 0
        @other_player_dir = 0

        intro_packet = ({
            "id_intro"=> "#{@player.player_id}"
        }).to_json
        @client.puts intro_packet

        @window = SDL::Window.new(title, width, height)
        @renderer = SDL::Renderer.new(@window)

        # look into <https://github.com/schovi/baked_file_system> for embedding files in binary

        @suicune = SDL::IMG.load("res/suicune.png")
        @suicune.alpha_mod = 88
        # @suicune = @suicune.convert(@window.surface)

        @running = true

        @suicune_x = 8
        @suicune_y = 8

        # tilemap
        tile_grid = [
            [0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 1, 0, 0, 0, 0, 2, 0, 0, 5, 0, 0],
            [0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0],
            [0, 0, 2, 0, 0, 0, 5, 0, 0, 0, 1, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 5, 0, 0, 0, 5, 1, 0, 0],
            [0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
            [0, 0, 0, 5, 2, 0, 0, 2, 0, 0, 0, 2, 2, 0, 2, 0, 0, 0, 0, 2, 0],
            [0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0],
            [1, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0],
            [0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 5, 1, 0, 0],
            [0, 1, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0],
            [0, 2, 0, 1, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0],
            [0, 0, 0, 2, 0, 0, 1, 2, 0, 1, 0, 0, 1, 2, 0, 0, 0, 2, 0, 0, 0],
            [0, 0, 0, 1, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0],
            [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 2, 0, 0, 0],
            [0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 5, 0, 0],
            [0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 2, 2, 0, 0, 1, 0, 5, 1, 0, 0, 0],
            [0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0],
            [2, 0, 2, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 5, 0],
            [2, 0, 2, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 5, 0],
            [2, 0, 2, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 5, 0]
        ]
        
        @tilemap = Tilemap.new(
                "res/tilemap.png", 
                tile_grid,
                32,
                5,
                [1, 2]
            );

        @player.set_collision_map(@tilemap.collision_map, 32)

        @camera = SDL::Rect.new(0,0,640,704)
    end

    def ingest_packet(data_packet : JSON::Any)
        packet = data_packet.as_h
        #puts @global_player_stats
        if packet.has_key?("player_stats")
            player_packet = packet["player_stats"].as_h
            player_packet.keys.each do |player_id|
                if player_id != "#{@player.player_id}"
                    stats = Hash(String, Int32).new

                    player_data = player_packet[player_id].as_h
                    player_data.each do |key, value|
                        if value.is_a?(Int32)
                            stats[key] = value
                        elsif value.is_a?(JSON::Any)
                            begin
                                stats[key] = value.as_i
                            rescue
                            end
                        end
                    end

                    @global_player_stats[player_id] = stats
                end
            end
        elsif packet.has_key?("disconnect")
            disconnected_player_id = packet["disconnect"].to_s
            @global_player_stats.delete(disconnected_player_id)
        end
    end

    def run
        while @running
            @currentTime = Time.monotonic

            delta_time = (@currentTime - @lastTime).milliseconds

            #@experiencedFps = (@experiencedFps * 0.9) + ((1000.0 / delta_time) * 0.1)
            @experiencedFps = (1000 / delta_time).to_f32.round(2)
            
            if delta_time > 1000/@fps

                data_packet = ({
                    player: {
                        "#{@player.player_id}"=> {
                            x: @player.x,
                            y: @player.y,
                            frame: @player.dir
                        },
                    }
                }).to_json

                @client.puts data_packet
                server_response = @client.gets || ""
                server_data_packet = JSON.parse(server_response)
                ingest_packet(server_data_packet)

                @window.title = "suicune 0.0.a [#{@experiencedFps} FPS]"
                @lastTime = @currentTime
                while event = SDL::Event.poll
                    

                    case event
                    when SDL::Event::Quit
                        @running = false
                    end

                    # player update
                    @player.update(event, @window.width, @window.height, delta_time.to_f / 1000)
                end


                clear_screen

               

                #puts "Camera:(#{@camera.x}, #{@camera.y})"
                
                

               # @camera.x = (@player.x - 320).clamp(0, @window.width)
               # @camera.y = (@player.y - 320).clamp(0, @window.height)
               # @renderer.viewport = @camera

                @tilemap.tile_grid.each_with_index do |row, i|
                    row.each_with_index do |value, j|
                        #@renderer.copy(@tilemap.tilemap_resource, @tilemap.resource_tiles[value], SDL::Rect[i*@tilemap.tile_size, j*@tilemap.tile_size, @tilemap.tile_size, @tilemap.tile_size])
                        @renderer.copy(@tilemap.tilemap_resource, @tilemap.resource_tiles[value], SDL::Rect[i*@tilemap.tile_size + @camera.x, j*@tilemap.tile_size + @camera.y, @tilemap.tile_size, @tilemap.tile_size])
                    end
                end

                # suicune splash
                @renderer.copy(@suicune, nil, SDL::Rect[0, 0, 32, 32])

                # player
                current_player_frame = @player.current_sprite_frame
                @renderer.copy(@player.sprite, current_player_frame, SDL::Rect[@player.x, @player.y, current_player_frame.w, current_player_frame.h])

                # other players
                @global_player_stats.each do |key, value|
                   puts value
                   # TODO: Update the width and height parameters here, these will have to be passed to client from server via other client's player object
                   # Ultimately the other players will be rendered beneath the player, but that can't happen until we remove current_player_frame dependency.
                   @renderer.copy(@other_player, @other_player_frames[value["frame"]], SDL::Rect[value["x"] || 150, value["y"] || 150, current_player_frame.w, current_player_frame.h])
                end


                @renderer.present
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
