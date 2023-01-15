# Unnamed 2D Game Engine

This game engine was created with the goal of further learning about game engine development. This is not intended to be used for any real projects.


## Dependencies

- SDL2 `2.0.14`
- SDL2_image `2.0.5`
- SDL2_ttf `2.0.15`
- SDL2_mixer `2.0.4`
- sol `3.2.1`
- lua `5.3`
- glm `0.9.9`
- dear imgui `1.83`
- pugixml `1.13`

## Progress Log

### Jan. 15, 2023

I added the basic Layers panel under the tileset on the right-side panel.

![Second Progress Pic](/screenshots/progress-2023-01-15.png)

### Jan. 10, 2023

Basically just started on the the tile editor part of this long journey.

Progress pic to mark this day
![First Progress Pic](/screenshots/progress-2023-01-10.png)


## TODO

- ~~Add tool selection~~
  - ~~Place Tile~~
  - ~~Erase Tile~~
  - ~~Flood fill~~
- ~~Add Editor State struct~~
  - ~~tool state~~
  - ~~tile selection state~~
  - ~~map tile data (tile size, map size in tiles, name, etc.)
      This map tile data will come from the XML map docs later on~~

- ~~Add undo/redo (Game Programming Patterns has a simple implementation in one of the chapters)~~
  - ~~Command pattern~~
- ~~Add a debounce time to commands~~
- ~~Add Mouse coords when over the canvas (both tile and pixel coords)~~
  - ~~Message bar at the bottom for coords~~
    - ~~Will eventually have messages for 'placed tile, ctrl+z to undo' type messages~~
- ~~Add Multi-layer support~~
  - ~~UI update for layers panel~~
  - ~~Currently selected layer~~
  - ~~Tile tool on a layer~~
- ~~Add pugixml and XML map file format~~
  - ~~Add pugixml~~
  - ~~Create file format~~
  - ~~Read file and turn it into a TileMap~~
  - ~~Document the file format~~
  - ~~Save TileMap into an XML file~~

- Add open/save
  - UI Dialog boxes to hook into previous work
- Add create new with settings
  - Blank TileMap initialization
  - UI Dialog box

- Add more ways to interact with the layers
  - Layer visible/hidden
  - Layer locked/unlocked

- Refactor tool dropdown to be a menu bar
  - Add Icon support

- Hotkeys
  - All the basic ctrl/cmd + __
  - Tool hotkeys (e for erase, b for place, f for fill, p for square fill)

- Expand Tools
  - Selection tools
    - Single/Square/Fill
  - Eye dropper tool

- UI Overhaul
  - Dynamic management of UI size/position
    - Resolution
    - Resizing
    - Sidebar size/position





- Start Game Editor
  - Merge in Game Engine
    - This will be a big one
    - The ECS stuff will need to be implemented, but only ran when the game is 'played'
    - The game needs to be able to be paused completely and also stopped/reset
    - The Game engine needs to be kinda of split from it's window implementation
    - So there should really be 3 'projects' -->
      - The editor -- Tool to create, edit and manage game files. Wraps the engine and runs it inside of the editor. loads up the
                    engines' assets/entities/etc. from the editor state. then starts it in a small part of the window.
      - The engine -- The core system that actually runs the game (ecs/game events/scripting/etc.)
      - The game   -- Creates the SDL stuff, passes it to the engine, loads up the engines' assets/entities/etc. from file then
                    starts it in the window. It would be nice if this was bundled by the editor with the assets inside the binary

  - Update Game Engine to read/parse map files
    - Add pugixml and map file format docs to Game Engine
    - Implement open and parse map file in Game Engine

  - Generate Game lua file
    - For the game: Basic game lua file pointing to the single 'level' aka the active tilemap
    - For the level: just the TileMap data for now, empty entities list
  - UI for Asset manager
    - Fonts
    - Images
  - Multiple Maps can be edited
    - Tabs in map editor
    - Saves multiple level files and correctly links to them in the game lua file
  - UI Entity list view
    - View all the entities in a level
  - Render all Entities
