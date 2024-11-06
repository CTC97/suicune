class Chunk
    property tiles : Array(Array(Int32))

    def initialize(x : Int32, y : Int32, range : Range(Int32, Int32))
        @tiles = Array.new(y) do
        Array.new(x) { rand(range) }
        end
    end

    def print
        @tiles.each do |row|
        puts row.join(" ")
        end
    end
end
