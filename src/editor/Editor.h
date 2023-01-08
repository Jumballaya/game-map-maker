#ifndef EDITOR_H
#define EDITOR_H

#include <memory>
#include <string>
#include <SDL2/SDL.h>
#include "../gui/GUI.h"

#include "../eventbus/EventBus.h"
#include "../events/Events.h"
#include "../assetstore/AssetStore.h"
#include "Canvas.h"
#include "../mouse/Mouse.h"
#include "../tilemap/Tilemap.h"

const int FPS = 30;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Editor
{
public:
  Editor();
  ~Editor();

  // Start/Close
  void initialize();
  void destroy();

  // Editor Loop
  void run();
  void setup();
  void processInput();
  void update();
  void render();

  //
  void loadMap(std::string filePath);

  // Events
  void onTileSelect(TileSelectEvent &event);

  // Editor
  static int windowWidth;
  static int windowHeight;

private:
  SDL_Window *window;
  SDL_Renderer *renderer;

  SDL_Texture *selectedTileset;

  // EditorState struct
  // stores main state
  //  current selected tile
  //  current tileset
  //  isRunning

  // MapCanvas class
  // Map data struct to be populated with map data
  // map tile data ---> tileSize, mapCols mapRows, mapWidth (px) mapHeight (px)
  // Need a draw method
  // Draws background, then mapdata
  // Multiple layers

  int tileRow;
  int tileCol;
  int tileSize;

  int mapWidth;
  int mapHeight;
  int imgWidth;
  int imgHeight;

  std::unique_ptr<AssetStore> assetStore;
  std::unique_ptr<EventBus> eventBus;
  std::unique_ptr<Mouse> mouse;
  std::unique_ptr<Canvas> canvas;
  std::unique_ptr<EditorGUI> gui;

  int millisPreviousFrame = 0;

  bool running;
  bool isDebug;
};

#endif