require "socket"

puts "suicune-server"

def handle_client(client)
  loop do
    message = client.gets
    puts message
    client.puts "ok."
  end
end

server = TCPServer.new("localhost", 1234)
while client = server.accept?
  spawn handle_client(client)
end
