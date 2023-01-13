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
  TileMap(glm::vec2 mapSize, int tileSize, double zoom);
  ~TileMap();

  int mapWidth;
  int mapHeight;
  int tileSize;
  double zoom;

  int width;           // Width of the map in pixels
  int height;          // Height of the map in pixels
  int colSize;         // Width of the map in tiles
  int rowSize;         // Height of the map in tiles
  glm::vec2 _tileSize; // dimensions of a single tile

  Tile getTile(glm::vec2 position) const;
  void updateTile(glm::vec2 position, glm::vec2 tileData);
  void floodFill(glm::vec2 position, glm::vec2 tileData);

  void setZoom(double zoom);

  void clear();
  void initialize(glm::vec2 mapSize, int tileSize);
  void draw(SDL_Renderer *renderer, SDL_Texture *texture, glm::vec2 position);

  // WITH LAYERS
  // Tile getTile(int layer, glm::vec2 position) const;
  // void updateTile(int layer, glm::vec2 position, glm::vec2 tileData);
  // void floodFill(int layer, glm::vec2 position, glm::vec2 tileData);
  // int createLayer(std::string name);
  // void renameLayer(int layerId, std::string name);
  // void deleteLayer(int layerId);
  // void moveLayer(int layerId, int layerIdToSwapWith);

private:
  std::vector<std::vector<Tile *>> tiles;

  bool inBounds(glm::vec2 position);
  void _floodFillRecursive(glm::vec2 position, glm::vec2 tileData, glm::vec2 targetValue, std::vector<int> *seen);

  // std::unordered_map<int, TileMapLayer> layers;
};

#endif