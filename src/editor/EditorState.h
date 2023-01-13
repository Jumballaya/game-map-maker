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
  int tileSize;
  int millisPreviousFrame = 0;
  glm::vec2 selectedTileData;
  glm::vec2 mapTileSize;
  glm::vec2 imageSize;
  SDL_Texture *selectedTileset;
  TileTool selectedTileTool;
  bool running;
  bool isDebug;

  Uint32 lastClick = 0;

  EditorState()
  {
    tileSize = 0;
    millisPreviousFrame = 0;
    selectedTileData = glm::vec2(0, 0);
    mapTileSize = glm::vec2(0, 0);
    imageSize = glm::vec2(0, 0);
    selectedTileset = NULL;
    selectedTileTool = TileTool::PlaceTile;
    running = false;
    isDebug = false;
  }
  ~EditorState() = default;
};

#endif