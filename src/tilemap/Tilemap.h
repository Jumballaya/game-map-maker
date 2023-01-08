#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <iostream>

struct Tile
{
public:
  Tile(int col, int row, int srcCol, int srcRow)
  {
    this->col = col;
    this->row = row;
    this->srcCol = srcCol;
    this->srcRow = srcRow;
  }

  int col;
  int row;
  int srcCol;
  int srcRow;

  friend std::ostream &operator<<(std::ostream &os, const Tile &t)
  {
    os << t.srcCol << t.srcRow;
    return os;
  }
};

class TileMap
{
public:
  TileMap(int mapWidth, int mapHeight, int tileSize);
  ~TileMap();

  int mapWidth;
  int mapHeight;
  int tileSize;

  Tile getTile(int row, int col) const;
  void updateTile(int row, int col, int srcRow, int srcCol);

private:
  std::vector<std::vector<Tile *>> tiles;
};

#endif