#ifndef FILLTILECOMMAND_H
#define FILLTILECOMMAND_H

#include "../../libs/glm/glm.hpp"

#include "./CommandManager.h"
#include "../tilemap/TileMap.h"

#include <memory>

class PlaceTileCommand : public ICommand
{
public:
  PlaceTileCommand(std::shared_ptr<TileMap> &tileMap, int layer, glm::vec2 targetTile, glm::vec2 tileValue)
  {
    this->tileMap = tileMap;
    this->targetTile = targetTile;
    this->tileValue = tileValue;
    this->layer = layer;
  };

  void execute()
  {
    Tile previousTile = tileMap->getTile(layer, targetTile);
    previousTileValue = glm::vec2(previousTile.srcCol, previousTile.srcRow);
    tileMap->updateTile(layer, targetTile, tileValue);
  }

  void undo()
  {
    tileMap->updateTile(layer, targetTile, previousTileValue);
  }

  void redo()
  {
    tileMap->updateTile(layer, targetTile, tileValue);
  }

private:
  std::shared_ptr<TileMap> tileMap;
  glm::vec2 targetTile;
  glm::vec2 tileValue;
  glm::vec2 previousTileValue;

  int layer;
};

#endif