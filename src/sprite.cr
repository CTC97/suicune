require "../lib/sdl/src/sdl"
require "../lib/sdl/src/image"

class Sprite
    property index : Int32

    property path : String
    property surface : SDL::Surface
    property frame_count : Int32
    property frames : Array(SDL::Rect)

    property animation_dict : Hash(String, Array(Int32))
    property current_animation : Array(Int32)
    property animation_speed : Float64
    property frame : Int32

    property sprite_name : String
  
    def initialize(@index : Int32, @path : String, @frame_count : Int32, @animation_speed)
        @surface = SDL::IMG.load("res/#{path}")
        @frames = Array(SDL::Rect).new(frame_count) do |i|
            # i * width, 0, width, height
            SDL::Rect.new(i * 32, 0, 32, 32)
        end

        @animation_dict = {} of String => Array(Int32)
        @current_animation = [] of Int32
        @frame = 0

        @sprite_name = path.split(".png")[0]
    end

    def frames
        @frames 
    end

    def define_animation(animation_frames : Array(Int32), name : String)
        @animation_dict[name] = animation_frames
    end

    def play_animation(name : String)
        @current_animation = @animation_dict[name]
    end

    def current_sprite_frame
        #@current_animation[(@frame * @animation_speed).to_i % @current_animation.size]
        @frames[@current_animation[(@frame * @animation_speed).to_i % @current_animation.size]]
    end

    # used for the frame in the sheet, NOT the frame in the animation
    def current_frame
        @current_animation[(@frame * @animation_speed).to_i % @current_animation.size]
    end

    def ==(other : Person) : Bool
      other.is_a?(Sprite) && path == other.path
    end
end