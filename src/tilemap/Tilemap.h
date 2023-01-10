#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <unordered_map>
#include <iostream>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

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

// class TileMapLayer
// {
// public:
//   TileMapLayer(int mapWidth, int mapHeight, int tileSize);
//   ~TileMapLayer();

//   // void draw(SDL_Renderer* renderer, SDL_Texture* texture, int originX, int originY, double zoom);
//   void draw()
//   {
//     // Render the map
//     // for (int y = 0; y < mapHeight; y++)
//     // {
//     //   int yPos = originY + (y * tileSize * zoom);
//     //   int ysrcRect = y * tileSize;
//     //   for (int x = 0; x < mapWidth; x++)
//     //   {
//     //     int xPos = originX + (x * tileSize * zoom);
//     //     int xSrcRect = x * tileSize;
//     //     SDL_Rect srcRect = {ysrcRect, xSrcRect, tileSize, tileSize};
//     //     SDL_Rect dstRect = {xPos, yPos, tileSize * zoom, tileSize * zoom};
//     //     SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
//     //   }
//     // }
//   }

// private:
//   std::vector<std::vector<Tile *>> tiles;
// };

class TileMap
{
public:
  TileMap(int mapWidth, int mapHeight, int tileSize);
  ~TileMap();

  int mapWidth;
  int mapHeight;
  int tileSize;

  Tile getTile(glm::vec2 position) const;
  void updateTile(glm::vec2 position, glm::vec2 tileData);

  void clear();
  void initialize(int mapWidth, int mapHeight, int tileSize);
  void draw(SDL_Renderer *renderer, SDL_Texture *texture, glm::vec2 position, double zoom);

  // Tile getTile(int layer, int row, int col) const;
  // void updateTile(int layer, int row, int col, int srcRow, int srcCol);

private:
  std::vector<std::vector<Tile *>> tiles;

  // std::unordered_map<int, TileMapLayer> layers;
};

#endif