#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <iostream>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

/**
 *
 *  Base Tile Class
 *
 *  Holds information about the tile location
 *
 *  @TODO: Maybe replace this with just an int index
 *
 */
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

/**
 *
 *  Tile Map Layer
 *
 *  A single layer of the tilemap, holds tile data for that layer
 *
 *  Interactions:
 *    Single tile placement
 *    Single tile erase
 *    Flood Fill
 *
 *
 */
class TileMapLayer
{
public:
  TileMapLayer(std::string name, int cols, int rows, int tileSize);
  ~TileMapLayer();

  void render(SDL_Renderer *renderer, SDL_Texture *texture, int xStart, int yStart, double zoom);

  Tile getTile(glm::vec2 position) const;
  void updateTile(glm::vec2 position, glm::vec2 tileData);
  void floodFill(glm::vec2 position, glm::vec2 tileData);

  void clear();
  void initialize(int cols, int rows, int tileSize);

  std::string name; // Name of the layer (for user)
  int rows;         // Number of rows
  int cols;         // Number of columns
  int tileSize;     // Size of a side of a tile

private:
  std::vector<std::vector<Tile *>> tiles;

  bool inBounds(glm::vec2 position);
  void _floodFillRecursive(glm::vec2 position, glm::vec2 tileData, glm::vec2 targetValue, std::vector<int> *seen);
};

/***
 *
 *  Full Tile Map
 *
 *  This is how you interact with individual layers and their tiles
 *
 *  Serializes to XML Map file, Deserialized from XML Map file
 *
 */
class TileMap
{
public:
  TileMap(glm::vec2 mapSize, int tileSize, double zoom);
  ~TileMap();

  double zoom;  // Mouse zoom amount
  int tileSize; // dimensions of a single tile
  int width;    // Width of the map in pixels
  int height;   // Height of the map in pixels
  int cols;     // Width of the map in tiles
  int rows;     // Height of the map in tiles

  void setZoom(double zoom);

  void clear();
  void initialize(glm::vec2 mapSize, int tileSize);
  void render(SDL_Renderer *renderer, SDL_Texture *texture, glm::vec2 position);

  Tile getTile(size_t layer, glm::vec2 position) const;
  void updateTile(size_t layer, glm::vec2 position, glm::vec2 tileData);
  void floodFill(size_t layer, glm::vec2 position, glm::vec2 tileData);

  int createLayer(std::string name);
  void renameLayer(size_t layerId, std::string name);
  void deleteLayer(size_t layerId);
  void moveLayer(size_t layerId, size_t layerIdToSwapWith);

  size_t layerCount() { return layers.size(); }

private:
  std::vector<TileMapLayer *> layers;
};

#endif