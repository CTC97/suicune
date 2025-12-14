## <img width="64" height="64" alt="suicune" src="https://github.com/user-attachments/assets/c538753c-3c3d-455a-a399-9c908e29326c" /> suicune [an approachable c++ game engine] <img width="64" height="64" alt="suicune" src="https://github.com/user-attachments/assets/c538753c-3c3d-455a-a399-9c908e29326c" />

### Ethos:
suicune is designed to make spinning up a c++ game quick and intuitive, prioritizing simple syntax but allowing for heavy customization.  

### Installation:
```
brew tap ctc97/suicune-tap
brew install suicune
```

### Creating a project:
Running the following command:
```
suicune_create myproject
```
Will create the following tree:
```
myproject
├── Makefile
├── main.cpp
├── res
├── src
│   ├── main_scene.cpp
│   └── main_scene.hpp
└── suicune_src
    ├── ...
```
You can then build your project by running:
```
make
build/suicune
```

---

### Repository Directory:

`example`: A working example of a suicune project with most existing functionalities implemented.

`scripts`: bash scripts used for suicune project creation and management.

`suicune_src`: the suicune engine source code, detailed below.
- `dialog_manager`: responsible for displaying in-game dialog, can be customized.
- `entity`: basic container for overworld entities
- `game`: a basic game container, manages `scenes`
- `menu_item`: an individual menu option
- `menu_scene`: a `scene` extension, contains `menu_items`
- `play_scene`: a `scene` extension that manages a player
- `player`: a basic player container, can/should be extended
- `scene`: a basic scene container, managed by a `game`
- `spritesheet`: manages images and animations
- `tilemap`: a basic tilemap container


