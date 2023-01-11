#ifndef FILLTILECOMMAND_H
#define FILLTILECOMMAND_H

#include "../../libs/glm/glm.hpp"

#include "./CommandManager.h"
#include "../tilemap/Tilemap.h"

#include <memory>

class FillTileCommand : public ICommand
{
public:
  FillTileCommand(std::shared_ptr<TileMap> &tileMap, glm::vec2 targetTile, glm::vec2 tileValue)
  {
    this->tileMap = tileMap;
    this->targetTile = targetTile;
    this->tileValue = tileValue;
  };

  void execute()
  {
    Tile previousTile = tileMap->getTile(targetTile);
    previousTileValue = glm::vec2(previousTile.col, previousTile.row);
    tileMap->updateTile(targetTile, tileValue);
  }

  void undo()
  {
    tileMap->updateTile(targetTile, previousTileValue);
  }

  void redo()
  {
    tileMap->updateTile(targetTile, tileValue);
  }

private:
  std::shared_ptr<TileMap> tileMap;
  glm::vec2 targetTile;
  glm::vec2 tileValue;
  glm::vec2 previousTileValue;
};

#endif