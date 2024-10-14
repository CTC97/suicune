require "socket"

require "../lib/sdl/src/sdl"
require "../lib/sdl/src/image"


class Client
    @suicune : SDL::Surface

    def initialize(title : String, width : Int32, height : Int32)
        SDL.init(SDL::Init::VIDEO)
        at_exit { SDL.quit }

        SDL::IMG.init(SDL::IMG::Init::PNG)
        at_exit { SDL::IMG.quit }

        @client = TCPSocket.new("localhost", 1234)

        @window = SDL::Window.new(title, width, height)
        @renderer = SDL::Renderer.new(@window)

        @suicune = SDL::IMG.load("res/suicune.png")
        # @suicune = @suicune.convert(@window.surface)

    end

    def run


        index = 0
        loop do
            case event = SDL::Event.wait
            when SDL::Event::Quit
                break
            end

            clear_screen
            @client << "#{index}\n"
            # index += 1
            # response = @client.gets
            # puts response
            @renderer.copy(@suicune, nil, nil)
            @renderer.present

            # @suicune.blit(@window.surface)
        end
    end

    private def clear_screen
        @renderer.draw_color = SDL::Color[208, 236, 234, 255] # clear screen in white
        @renderer.clear
    end
end

client = Client.new("Suicune", 640, 480)
client.run
