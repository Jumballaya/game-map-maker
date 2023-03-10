#ifndef EDITOR_H
#define EDITOR_H

#include <memory>
#include <string>
#include <SDL2/SDL.h>
#include <sol/sol.hpp>

#include "../gui/GUI.h"
#include "../eventbus/EventBus.h"
#include "../assetstore/AssetStore.h"
#include "Canvas.h"
#include "./EditorState.h"
#include "../mouse/Mouse.h"
#include "../tilemap/TileMap.h"
#include "../commands/CommandManager.h"

#include "../events/Events.h"
#include "../commands/Commands.h"

const int FPS = 30;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Editor
{
public:
  Editor();
  ~Editor();

  // Start/Close
  void initialize(const std::string &rootPath);
  void destroy();

  // Editor Loop
  void run();
  void setup();
  void processInput();
  void update();
  void render();

  //
  void loadMap(std::string filePath);

  // Drawing Tools
  void renderCanvasCursor();
  void placeTile();
  void eraseTile();
  void floodFill();

  // Events
  void onTileSelect(TileSelectEvent &event);
  void onTileToolSelect(TileToolSelectEvent &event);
  void onRunLua(RunLUAEvent &event);
  void onTileMapLayerSelect(TileMapLayerSelectEvent &event);
  void onTileMapLayerNameChange(TileMapLayerNameChangeEvent &event);
  void onTileSetSelect(TileSetSelectEvent &event);
  void onOpenTileMap(OpenTileMapEvent &event);
  void onSaveTileMap(SaveTileMapEvent &event);
  void onCreateNewTileMap(CreateNewTileMapEvent &event);
  void onAddTileSet(AddTileSetEvent &event);

  // Editor
  static int windowWidth;
  static int windowHeight;

private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  EditorState state;

  std::shared_ptr<TileMap> tileMap;
  std::unique_ptr<AssetStore> assetStore;
  std::unique_ptr<EventBus> eventBus;
  std::unique_ptr<Mouse> mouse;
  std::unique_ptr<Canvas> canvas;
  std::unique_ptr<EditorGUI> gui;
  std::unique_ptr<CommandManager> commandManager;

  sol::state lua;

  std::string rootPath;
};

#endif