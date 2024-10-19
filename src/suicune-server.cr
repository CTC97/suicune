require "socket"
require "json"

puts "suicune-server"

class Server

    @player_stats : Hash(String, Hash(String, Int32))
    @client_id_map : Hash(String, String)

    def initialize
        @clients = [] of TCPSocket
        @client_id_map = Hash(String, String).new
        @player_stats = Hash(String, Hash(String, Int32)).new
    end

    def ingest_packet(data_packet : JSON::Any, client_id : String)
        packet = data_packet.as_h
        if packet.has_key?("player")
            player_packet = packet["player"].as_h
            player_id = player_packet.keys.first
            player_data = player_packet[player_id].as_h

             # Create a new hash for the player's stats
            stats = Hash(String, Int32).new

            # Iterate over the keys in player_data and assign them to stats
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

            @player_stats[player_id] = stats

        elsif packet.has_key?("id_intro")
            @client_id_map[client_id] = packet["id_intro"].to_s
            puts @client_id_map
        end
    end

    def handle_disconnect(client)
        @clients.delete(client)

        player_id = @client_id_map["#{client.remote_address}"]?
        @client_id_map.delete("#{client.remote_address}")
        @player_stats.delete(player_id)
        puts "Player #{player_id} disconnected"

        disconnection_message = ({
            "disconnect" => "#{player_id}"
        }).to_json
        puts disconnection_message
        send_data_to_clients(disconnection_message)

        client.close
    end

    def handle_client(server, client)
        begin
            loop do
                data_packet_string = client.gets || ""
                if data_packet_string != ""
                    data_packet = JSON.parse(data_packet_string)
                    ingest_packet(data_packet, "#{client.remote_address}")
                end

                client.puts ({"player_stats" => @player_stats}).to_json
            end
        rescue IO::Error
            handle_disconnect(client)
        end
    end

    def send_data_to_clients(map)
        @clients.each do |client|
            begin
                client.puts(map)
            rescue IO::Error
                handle_disconnect(client)
            end
        end
    end

    def start
        server = TCPServer.new("localhost", 1234)
        server.reuse_address=true
        server.reuse_address=true
        server.reuse_port=true
        #reuse_port? : Bool
        while client = server.accept?
            @clients << client
            spawn handle_client(server, client)
            #end
        end
    end
end

server = Server.new()
server.start
