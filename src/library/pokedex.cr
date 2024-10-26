class Pokedex

    @@pokemon_sprites : Hash(Int32, Sprite)

    @@igglybuff = Sprite.new(174, "igglybuff.png", 16, 0.12)
    @@igglybuff.define_animation([2,3], "left")
    @@igglybuff.define_animation([4,5], "right")
    @@igglybuff.define_animation([0,1], "down")
    @@igglybuff.define_animation([6,7], "up")

    @@cleffa = Sprite.new(173, "cleffa.png", 16, 0.12)
    @@cleffa.define_animation([2,3], "left")
    @@cleffa.define_animation([4,5], "right")
    @@cleffa.define_animation([0,1], "down")
    @@cleffa.define_animation([6,7], "up")

    @@togepi = Sprite.new(175, "togepi.png", 16, 0.16)
    @@togepi.define_animation([4,5,6,7], "left")
    @@togepi.define_animation([8,9,10,11], "right")
    @@togepi.define_animation([0,1,2,3], "down")
    @@togepi.define_animation([12,13,14,15], "up")

    @@pokemon_sprites = {
        174 => @@igglybuff,
        173=> @@cleffa,
        175 => @@togepi,
    }

    def self.pokemon_sprites
        @@pokemon_sprites
    end
end