#include "./Tilemap.h"
#include <algorithm>
#include <cmath>

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
  this->mapWidth = mapSize.x;
  this->mapHeight = mapSize.y;
  this->tileSize = tileSize;
  for (int y = 0; y < mapHeight; y++)
  {
    std::vector<Tile *> row;
    tiles.push_back(row);
    for (int x = 0; x < mapWidth; x++)
    {
      Tile *t = new Tile(x, y, -1, -1);
      tiles[y].push_back(t);
    }
  }
}

Tile TileMap::getTile(glm::vec2 position) const
{
  int col = position.x;
  int row = position.y;

  if (row < static_cast<int>(tiles.size()) && col < static_cast<int>(tiles[row].size()))
  {
    return *tiles[row][col];
  }
  Tile def(-1, -1, -1, -1);
  return def;
};

void TileMap::updateTile(glm::vec2 position, glm::vec2 tileData)
{
  int col = position.x;
  int row = position.y;
  int srcCol = tileData.x;
  int srcRow = tileData.y;

  if (inBounds(position))
  {
    tiles[row][col]->srcCol = srcCol;
    tiles[row][col]->srcRow = srcRow;
  }
};

void TileMap::floodFill(glm::vec2 position, glm::vec2 tileData)
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
      int row = std::floor(pos / mapWidth);
      int col = pos % mapWidth;
      updateTile(glm::vec2(col, row), tileData);
    }

    seen.clear();
  }
}

void TileMap::_floodFillRecursive(glm::vec2 position, glm::vec2 tileData, glm::vec2 targetValue, std::vector<int> *seen)
{
  int col = position.x;
  int row = position.y;
  int srcCol = tileData.x;
  int srcRow = tileData.y;

  if (inBounds(position))
  {
    glm::vec2 currentValue = glm::vec2(tiles[row][col]->srcCol, tiles[row][col]->srcRow);
    bool targetTileSame = currentValue.x == targetValue.x && currentValue.y == targetValue.y;
    bool haveSeenTile = std::count(seen->begin(), seen->end(), col + (row * mapWidth));

    if (targetTileSame && !haveSeenTile)
    {
      // fill the tile
      tiles[row][col]->srcCol = srcCol;
      tiles[row][col]->srcRow = srcRow;

      // add to seen vector
      seen->push_back(col + (row * mapWidth));

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

void TileMap::clear()
{
  for (int y = 0; y < mapHeight; y++)
  {
    for (int x = 0; x < mapWidth; x++)
    {
      delete tiles[y][x];
    }
    tiles[y].clear();
  }
  tiles.clear();
}

void TileMap::draw(SDL_Renderer *renderer, SDL_Texture *texture, glm::vec2 position)
{
  // Render the map
  int y = 0;
  int x = 0;
  int xStart = position.x;
  int yStart = position.y;
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

void TileMap::setZoom(double zoom)
{
  this->zoom = zoom;
}

bool TileMap::inBounds(glm::vec2 position)
{
  bool inRowBounds = position.y >= 0 && position.y < static_cast<int>(tiles.size());
  if (!inRowBounds)
  {
    return false;
  }

  bool inColBounds = position.x >= 0 && position.x < static_cast<int>(tiles[0].size());
  return inRowBounds && inColBounds;
}
