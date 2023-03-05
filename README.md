# Game Map Maker

This game map maker was created with the goal of further learning about game engine development. This is not intended to be used for any real projects.


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

- ~~Add Dialog boxes (Open, New, Save)~~
  - ~~Add Open TileMap~~ 
  - ~~Add create new with settings~~
    - ~~Blank TileMap initialization~~
    - ~~UI Dialog box~~
  - ~~Add Save dialog box~~

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
    - Use ImGui 'docking' branch
