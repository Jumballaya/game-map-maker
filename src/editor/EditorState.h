#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include <glm/glm.hpp>

enum class TileTool
{
  PlaceTile,
  EraseTile,
  FloodFill,
};

struct EditorState
{
public:
  int millisPreviousFrame = 0;

  // glm::vec2 mapTileSize; // TileMap.sizeTile

  std::string selectedTileset; // Name of selected tile set
  glm::vec2 selectedTileData;  // The selected tile (col, row)
  size_t selectedLayer;        // The layerId of currently selected layer

  TileTool selectedTileTool;
  bool running;
  bool isDebug;

  int windowWidth;
  int windowHeight;

  bool hoveringCanvas;
  glm::vec2 hoveringCoords;

  bool dialogOpen = false;

  Uint32 lastClick = 0; // For mouse click debounce

  EditorState()
  {
    millisPreviousFrame = 0;
    selectedTileData = glm::vec2(0, 0);
    selectedLayer = 0;
    selectedTileset = "";
    selectedTileTool = TileTool::PlaceTile;
    running = false;
    isDebug = false;
    hoveringCoords = glm::vec2(0, 0);
    hoveringCanvas = false;
  }
  ~EditorState() = default;
};

#endif