#pragma once

#include "../suicune_src/play_scene.hpp"

namespace suicune
{

    class DreamScene : public PlayScene
    {
    public:
        DreamScene(Game &game);
        ~DreamScene() override;

        void draw() override;
        void update(float dt) override;

    protected:
        Camera2D &get_camera() override;

    private:
        Camera2D camera{};
        std::unique_ptr<Spritesheet> tilemap_spritesheet;
        std::unique_ptr<Spritesheet> nurse_joy_spritesheet;
        std::unique_ptr<Spritesheet> player_spritesheet;

        std::array<std::string, 10> dialogue_lines{
            {"You’re early… or maybe I’m late again. The clocks here don’t argue, they just sigh and move on without us. I tried setting mine back once, but it looked offended, so I apologized. If you hear a ticking in your pockets later, don’t panic. It’s only borrowing your time for a while.",

             "I had a name this morning, I’m fairly sure of it. It fit better before noon, though. Names tend to loosen as the day warms up, like buttons on an old coat. If you remember it later, you’re welcome to bring it back. I’ll trade you a story I haven’t finished yet.",

             "They told me the well answers questions, but only the ones you didn’t mean to ask. I leaned over once and heard my own voice answering back, slower, more patient. Haven’t been down there since, though I wave sometimes when I pass. It feels rude not to.",

             "Careful where you stand. The ground remembers feet, and some memories ache more than others. Right there, yes, that’s where someone waited too long for a promise. If your shoes feel heavier, it’s not your fault. The town likes to hold onto things.",

             "I sell candles, but not for light. These are for keeping shadows occupied so they don’t wander off. One burned all the way down last night and I spent hours looking for my reflection afterward. If you hear whispering when you light it, just thank it and close the door.",

             "You look like someone who almost remembers something important. Don’t rush it. Memories here arrive like guests who missed the invitation but brought wine anyway. Sit a moment. If it’s meant to stay, it will.",

             "The birds aren’t late, you know. They just leave before the sky finishes waking up. I tried following once and lost an afternoon between two clouds. Lovely place, but nothing grows there except excuses. I keep one in my pocket, just in case.",

             "Every house in this town is bigger on the inside, emotionally speaking. Walls learn things. Doors, too. Mine sighs every time I open it, like it’s tired of pretending not to recognize me. If yours does the same, you’re settling in nicely.",

             "I used to collect footsteps. Kept them in jars, labeled by season and mood. Autumn steps were the loudest, always in a hurry to become leaves. Someone knocked them all over during a storm, and now the nights pace around on their own. Listen closely, you can hear them practicing.",

             "Oh, you can leave whenever you want. The roads won’t stop you. They’ll just take their time deciding where to go. If you find yourself walking in a circle, that’s not a mistake. That’s the town making sure you’ve seen enough of yourself to continue."}};
    };

}