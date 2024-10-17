require "socket"
require "json"

puts "suicune-server"

class Server

    def initialize
        @clients = [] of TCPSocket
        @local_data_packet = ({
            old_man: {
                x: (32 * 3),
                y: (32 * 3)
            }
        }).to_json
    end

    def handle_client(server, client)
    loop do
        data_packet_string = client.gets || ""

        #puts data_packet_string
        #puts typeof(data_packet_string)
        #data_packet = JSON.parse(data_packet_string)
        #puts data_packet
        #puts typeof(data_packet)
        #puts data_packet["player"]?

        client.puts @local_data_packet
    end
    end



    def start
        server = TCPServer.new("localhost", 1234)
        while client = server.accept?
            @clients << client
            @clients.each do |client|
                spawn handle_client(server, client)
            end
        end
    end
end

server = Server.new()
server.start
