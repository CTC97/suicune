require "socket"
require "json"

require "./sprite.cr"


require "./player"
require "./tilemap"
require "./chunk"

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
    @render_distance : Int32

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

        @other_player = SDL::IMG.load("res/igglybuff.png")
        @other_player_frames = StaticArray(SDL::Rect, 8).new do |i|
            # i * width, 0, width, height
            SDL::Rect.new(i * 32, 0, 32, 32)
        end
        
        @render_distance = 5

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
        # TO DO : SIGNIFICANT PERFORMANCE DROPOFF AFTER 32, MUST FIND OUT WHY, THIS WONT DO
        chunk = Chunk.new(400, 400, 0..5)
        
        @tilemap = Tilemap.new(
                "res/tilemap.png", 
                chunk.tiles,
                32,
                5,
                [1, 2]
            );

        @player.set_collision_map(@tilemap.collision_map, @tilemap.tile_size)

        @camera = SDL::Rect.new(0,0,320,352)
    end

    def ingest_packet(data_packet : JSON::Any)
        
        packet = data_packet.as_h
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
                            sprite: @player.poke_sprite.index,
                            x: @player.x,
                            y: @player.y,
                            frame: @player.current_frame
                        },
                    }
                }).to_json

                @client.puts data_packet
                server_response = @client.gets || ""
                server_data_packet = JSON.parse(server_response)
                ingest_packet(server_data_packet)

                @window.title = "suicune 0.0.a [#{@experiencedFps} FPS]"
                @lastTime = @currentTime
                event = SDL::Event.poll
                    

                case event
                when SDL::Event::Quit
                    @running = false
                end

                # player update
                @player.update(event, @window.width, @window.height, delta_time.to_f / 1000)
            


                clear_screen

               

                #puts "Camera:(#{@camera.x}, #{@camera.y})"
                
                
                # https://www.youtube.com/watch?v=QeN1ygJD5y4&ab_channel=Let%27sMakeGames
               # @camera.x = (@player.x - 320).clamp(0, @window.width)
               # @camera.y = (@player.y - 320).clamp(0, @window.height)
               # @renderer.viewport = @camera
               # 
               @renderer.scale = {@player.scale, @player.scale}
               

               smooth_factor = 0.9 # Adjust this value for different smoothness levels (0.1 is smooth, closer to 1 is snappier)

               @camera.x = @player.x - (@window.width / (2 * @player.scale)).to_i
               @camera.y = @player.y - (@window.height / (2 * @player.scale)).to_i
               
               #@camera.x = lerp(@camera.x, target_camera_x, smooth_factor)
               #@camera.y = lerp(@camera.y, target_camera_y, smooth_factor)

                #@renderer.viewport = @camera

              # puts "#{@camera.x}, #{@camera.y}"
                
               


                @tilemap.tile_grid.each_with_index do |row, i|
                    row.each_with_index do |value, j|
                        if ((j - @player.tile_x).abs <= @render_distance && (i - @player.tile_y).abs <= @render_distance)
                            @renderer.copy(@tilemap.tilemap_resource, @tilemap.resource_tiles[value], SDL::Rect[j*@tilemap.tile_size - @camera.x, i*@tilemap.tile_size - @camera.y, @tilemap.tile_size, @tilemap.tile_size])
                        end
                    end
                end

                # suicune splash
                

                #puts "camera: #{@camera.x/32}, #{@camera.y/32}"

                @global_player_stats.each do |key, value|

                    sprite_to_render = Pokedex.pokemon_sprites[value["sprite"]]
                    sprite_frame = sprite_to_render.frames[value["frame"]]
                    other_player_x = value["x"] - @camera.x
                    other_player_y = value["y"] - @camera.y
                    #puts "#{(value["x"] / @tilemap.tile_size)}, #{(value["y"] / @tilemap.tile_size)}"
                    if (((value["x"]/(@tilemap.tile_size)) - @player.tile_x).abs <= @render_distance && ((value["y"]/(@tilemap.tile_size)) - @player.tile_y).abs <= @render_distance)
                        @renderer.copy(sprite_to_render.surface, sprite_frame, SDL::Rect[other_player_x, other_player_y, sprite_frame.w, sprite_frame.h])
                    end
                end

                # player
                current_player_frame = @player.current_sprite_frame
                @renderer.copy(@player.sprite, current_player_frame, SDL::Rect[@player.x - @camera.x, @player.y - @camera.y, current_player_frame.w, current_player_frame.h])


                @renderer.scale = {2, 2}
                @renderer.copy(@suicune, nil, SDL::Rect[0, 0, 32, 32])
                #puts "current player: #{@player.x/32}, #{@player.y/32}"
                # other players
                
                
                
                @renderer.present
            end
        end
    end

    def lerp(start, target, t)
        (start + (target - start) * t).to_i
    end

    private def clear_screen
        @renderer.draw_color = SDL::Color[132, 90, 255, 255]
        @renderer.clear
    end
end

client = Client.new("suicune v0.0.1", 640, 640, 60)
client.run
