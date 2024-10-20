require "../lib/sdl/src/sdl"
require "../lib/sdl/src/image"

class Tilemap

    @tilemap_resource : SDL::Surface
    @tile_grid : Array(Array(Int32))
    @collision_tiles : Array(Int32)
    @collision_map : Array(Array(Bool))
    @tile_count : Int32
    @tile_size : Int32
    @resource_tiles :  Array(SDL::Rect)

    def initialize(
            tilemap_path : String, 
            tile_grid : Array(Array(Int32)), 
            tile_size : Int32,
            tile_count : Int32,
            collision_tiles : Array(Int32)
        )

        @tilemap_resource = SDL::IMG.load("res/tilemap.png")
        @tile_grid = tile_grid
        @tile_size = tile_size
        @tile_count = tile_count
        @collision_tiles = collision_tiles

        @resource_tiles = Array(SDL::Rect).new(6) do |i|
            # params here represent - (i * w, 0, w, h)
            SDL::Rect.new(i * @tile_size, 0, @tile_size, @tile_size)
        end

        rows = @tile_grid.size
        cols = @tile_grid[0].size

        @collision_map = Array(Array(Bool)).new(rows) { Array(Bool).new(cols, false) }
        @tile_grid.each_with_index do |row, row_index|
            row.each_with_index do |tile, col_index|
                @collision_map[row_index][col_index] = @collision_tiles.includes?(tile)
            end
        end
    end

    def check_collision(x : Int32, y : Int32)
        @collision_map[x][y]
    end

    def collision_map
        @collision_map
    end

    def tilemap_resource
        @tilemap_resource
    end

    def tile_grid
        @tile_grid
    end 
    
    def collision_tiles
        @collision_tiles
    end

    def resource_tiles
        @resource_tiles
    end

    def tile_size
        @tile_size
    end

end

