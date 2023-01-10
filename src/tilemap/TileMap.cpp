#include "./Tilemap.h"

TileMap::TileMap(int mapWidth, int mapHeight, int tileSize)
{
  initialize(mapWidth, mapHeight, tileSize);
};

TileMap::~TileMap()
{
  clear();
}

void TileMap::initialize(int mapWidth, int mapHeight, int tileSize)
{
  this->mapWidth = mapWidth;
  this->mapHeight = mapHeight;
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

  if (row < tiles.size() && col < tiles[row].size())
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

  if (row < tiles.size() && col < tiles[row].size())
  {
    tiles[row][col]->srcCol = srcCol;
    tiles[row][col]->srcRow = srcRow;
  }
};

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

void TileMap::draw(SDL_Renderer *renderer, SDL_Texture *texture, glm::vec2 position, double zoom)
{
  // Render the map
  int y = 0;
  int x = 0;
  int width = static_cast<int>(mapWidth * tileSize * zoom);
  int height = static_cast<int>(mapHeight * tileSize * zoom);
  int xStart = position.x - (width / 2);
  int yStart = position.y - (height / 2);
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
      SDL_Rect dstRect = {xPos, yPos, tileSize * zoom, tileSize * zoom};
      SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);

      x++;
    }
    y++;
    x = 0;
  }
}