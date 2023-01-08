#include "./Tilemap.h"

TileMap::TileMap(int mapWidth, int mapHeight, int tileSize)
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
};

TileMap::~TileMap()
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

Tile TileMap::getTile(int row, int col) const
{
  if (row < tiles.size() && col < tiles[row].size())
  {
    return *tiles[row][col];
  }
  Tile def(-1, -1, -1, -1);
  return def;
};

void TileMap::updateTile(int row, int col, int srcRow, int srcCol)
{
  if (row < tiles.size() && col < tiles[row].size())
  {
    tiles[row][col]->srcCol = srcCol;
    tiles[row][col]->srcRow = srcRow;
  }
};
