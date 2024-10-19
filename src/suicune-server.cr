require "socket"
require "json"

puts "suicune-server"

class Server

    @player_stats : Hash(String, Hash(String, Int32))

    def initialize
        @clients = [] of TCPSocket

        @player_stats = Hash(String, Hash(String, Int32)).new
    end

    def ingest_packet(data_packet : JSON::Any)
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
        end
    end

    def handle_client(server, client)
       loop do
            data_packet_string = client.gets || ""
            #puts data_packet_string
            if data_packet_string != ""
                puts "\tparsing"
                data_packet = JSON.parse(data_packet_string)
                ingest_packet(data_packet)
            end

            #puts data_packet_string
            #puts typeof(data_packet_string)
            #data_packet = JSON.parse(data_packet_string)
            #puts data_packet
            #puts typeof(data_packet)
            #puts data_packet["player"]?

            puts ({"player_stats" => @player_stats}).to_json
            client.puts ({"player_stats" => @player_stats}).to_json
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
            #@clients.each do |client|
            spawn handle_client(server, client)
            #end
        end
    end
end

server = Server.new()
server.start
