#include <algorithm>
#include <cmath>

#include "./TileMap.h"
#include "../assetstore/AssetStore.h"

int create_index(int x, int y, int cols)
{
  if (x == -1 && y == -1)
  {
    return 0;
  }
  return (y * cols) + x + 1;
}

/****
 *
 *  Tile Map Layer
 *
 */

TileMapLayer::TileMapLayer(std::string name, std::string tileset, int cols, int rows, int tileSize, int tilesetCols)
{
  this->name = name;
  this->tileset = tileset;
  this->tilesetCols = tilesetCols;
  initialize(cols, rows, tileSize);
}

TileMapLayer::~TileMapLayer()
{
  clear();
}

void TileMapLayer::initialize(int cols, int rows, int tileSize)
{
  this->cols = cols;
  this->rows = rows;
  this->tileSize = tileSize;
  for (int y = 0; y < rows; y++)
  {
    std::vector<Tile *> row;
    tiles.push_back(row);
    for (int x = 0; x < cols; x++)
    {
      Tile *t = new Tile(x, y, -1, -1, -1);
      tiles[y].push_back(t);
    }
  }
}

Tile TileMapLayer::getTile(glm::vec2 position) const
{
  int col = position.x;
  int row = position.y;

  if (row < static_cast<int>(tiles.size()) && col < static_cast<int>(tiles[row].size()))
  {
    return *tiles[row][col];
  }
  Tile def(-1, -1, -1, -1, -1);
  return def;
};

void TileMapLayer::updateTile(glm::vec2 position, glm::vec2 tileData)
{
  int col = position.x;
  int row = position.y;
  int srcCol = tileData.x;
  int srcRow = tileData.y;
  int index = create_index(tileData.x, tileData.y, tilesetCols);

  if (inBounds(position))
  {
    tiles[row][col]->srcCol = srcCol;
    tiles[row][col]->srcRow = srcRow;
    tiles[row][col]->index = index;
  }
};

void TileMapLayer::floodFill(glm::vec2 position, glm::vec2 tileData)
{
  int col = position.x;
  int row = position.y;
  if (inBounds(position))
  {
    // Get the value of the tile clicked on, fill only connected tiles with that value
    glm::vec2 targetValue = glm::vec2(tiles[row][col]->srcCol, tiles[row][col]->srcRow);

    std::vector<int> seen;
    _floodFillRecursive(position, tileData, targetValue, &seen);

    for (auto pos : seen)
    {
      int row = std::floor(pos / cols);
      int col = pos % cols;
      updateTile(glm::vec2(col, row), tileData);
    }

    seen.clear();
  }
}

void TileMapLayer::_floodFillRecursive(glm::vec2 position, glm::vec2 tileData, glm::vec2 targetValue, std::vector<int> *seen)
{
  int col = position.x;
  int row = position.y;
  int srcCol = tileData.x;
  int srcRow = tileData.y;

  if (inBounds(position))
  {
    glm::vec2 currentValue = glm::vec2(tiles[row][col]->srcCol, tiles[row][col]->srcRow);
    bool targetTileSame = currentValue.x == targetValue.x && currentValue.y == targetValue.y;
    bool haveSeenTile = std::count(seen->begin(), seen->end(), col + (row * cols));

    if (targetTileSame && !haveSeenTile)
    {
      // fill the tile
      tiles[row][col]->srcCol = srcCol;
      tiles[row][col]->srcRow = srcRow;

      // add to seen vector
      seen->push_back(col + (row * cols));

      // Loop through and fill all connected tiles (to the immediately l/r t/b)
      glm::vec2 left = glm::vec2(position.x - 1, position.y);
      glm::vec2 right = glm::vec2(position.x + 1, position.y);
      glm::vec2 top = glm::vec2(position.x, position.y - 1);
      glm::vec2 bottom = glm::vec2(position.x, position.y + 1);

      _floodFillRecursive(left, tileData, targetValue, seen);
      _floodFillRecursive(right, tileData, targetValue, seen);
      _floodFillRecursive(top, tileData, targetValue, seen);
      _floodFillRecursive(bottom, tileData, targetValue, seen);
    }
  };
}

void TileMapLayer::clear()
{
  for (int y = 0; y < rows; y++)
  {
    for (int x = 0; x < cols; x++)
    {
      delete tiles[y][x];
    }
    tiles[y].clear();
  }
  tiles.clear();
}

void TileMapLayer::render(SDL_Renderer *renderer, SDL_Texture *texture, int xStart, int yStart, double zoom)
{
  int x = 0;
  int y = 0;
  for (auto row : tiles)
  {
    for (auto col : row)
    {
      if (col->srcRow == -1 || col->srcCol == -1)
      {
        x++;
        continue;
      }

      int yPos = static_cast<int>(yStart + (y * tileSize * zoom));
      int ySrcRect = col->srcRow * tileSize;
      int xPos = static_cast<int>(xStart + (x * tileSize * zoom));
      int xSrcRect = col->srcCol * tileSize;
      SDL_Rect srcRect = {xSrcRect, ySrcRect, tileSize, tileSize};
      SDL_Rect dstRect = {xPos, yPos, static_cast<int>(tileSize * zoom), static_cast<int>(tileSize * zoom)};
      SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);

      x++;
    }
    y++;
    x = 0;
  }
}

bool TileMapLayer::inBounds(glm::vec2 position)
{
  bool inRowBounds = position.y >= 0 && position.y < static_cast<int>(tiles.size());
  if (!inRowBounds)
  {
    return false;
  }

  bool inColBounds = position.x >= 0 && position.x < static_cast<int>(tiles[0].size());
  return inRowBounds && inColBounds;
}

// End Tile Map Layer

/***
 *
 *  Tile Map
 *
 *
 */
TileMap::TileMap(glm::vec2 mapSize, int tileSize, double zoom = 1.0)
{
  initialize(mapSize, tileSize);
};

TileMap::~TileMap()
{
  clear();
}

void TileMap::initialize(glm::vec2 mapSize, int tileSize)
{
  clear();
  this->tileSize = tileSize;
  cols = mapSize.x;
  rows = mapSize.y;
  width = mapSize.x * tileSize;
  height = mapSize.y * tileSize;
}

Tile TileMap::getTile(size_t layer, glm::vec2 position) const
{
  if (layer < layers.size())
  {
    return layers[layer]->getTile(position);
  }
  return Tile(-1, -1, -1, -1, -1);
};

void TileMap::updateTile(size_t layer, glm::vec2 position, glm::vec2 tileData)
{
  if (layer < layers.size())
  {
    return layers[layer]->updateTile(position, tileData);
  }
};

void TileMap::floodFill(size_t layer, glm::vec2 position, glm::vec2 tileData)
{
  if (layer < layers.size())
  {
    return layers[layer]->floodFill(position, tileData);
  }
}

void TileMap::clear()
{
  for (auto layer : layers)
  {
    layer->clear();
  }
}

void TileMap::render(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore, glm::vec2 position)
{
  int xStart = position.x;
  int yStart = position.y;
  for (auto layer : layers)
  {
    layer->render(renderer, assetStore->getTileset(layer->tileset)->getTexture(), xStart, yStart, zoom);
  }
}

void TileMap::setZoom(double zoom)
{
  this->zoom = zoom;
}

size_t TileMap::createLayer(const std::string &name, const std::string &tileset, int tilesetCols)
{
  TileMapLayer *layer = new TileMapLayer(name, tileset, cols, rows, tileSize, tilesetCols);
  layers.push_back(layer);
  return layers.size() - 1;
};

void TileMap::renameLayer(size_t layerId, const std::string &name)
{
  if (layerId >= 0 && layerId < layers.size())
  {
    layers[layerId]->name = name;
  }
};

void TileMap::deleteLayer(size_t layerId)
{
  if (layerId < 0 || layerId > layers.size() - 1)
  {
    return;
  }

  // Delete last layer
  if (layerId == layers.size() - 1)
  {
    auto last = layers[layerId];
    last->clear();
    layers.pop_back();
    delete last;
    return;
  }

  // Any other layer
  auto layerToDelete = layers[layerId];
  layerToDelete->clear();
  for (size_t i = layerId; i < layers.size() - 1; i++)
  {
    layers[i] = layers[i + 1];
  }
  layers.pop_back();
};

void TileMap::moveLayer(size_t layerId, size_t layerIdToMoveTo)
{
  if (layerId < 0 || layerId > layers.size() - 1)
  {
    return;
  }
  if (layerIdToMoveTo < 0 || layerIdToMoveTo > layers.size() - 1)
  {
    return;
  }

  auto layerToMove = layers[layerId];
  for (size_t i = layerId; i < layerIdToMoveTo; i++)
  {
    layers[i] = layers[i + 1];
  }
  layers[layerIdToMoveTo] = layerToMove;
  return;
};

const std::string &TileMap::getLayerName(size_t layerId) const
{
  if (layerId < layers.size())
  {
    return layers[layerId]->name;
  }
  return NULL;
};

void TileMap::swapLayers(size_t layer1, size_t layer2)
{
  if (layer1 < 0 || layer1 > layers.size() - 1)
  {
    return;
  }
  if (layer2 < 0 || layer2 > layers.size() - 1)
  {
    return;
  }
  auto temp1 = layers[layer1];
  auto temp2 = layers[layer2];
  layers[layer2] = temp1;
  layers[layer1] = temp2;
};

TileMapLayer *TileMap::getLayer(size_t layerId)
{
  if (layerId < 0 || layerId > layers.size() - 1)
  {
    return nullptr;
  }
  return layers[layerId];
};
