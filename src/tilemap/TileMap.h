#ifndef TILEMAP_H
#define TILEMAP_H

#include <memory>
#include <vector>
#include <iostream>

#include <SDL2/SDL.h>
#include "../../libs/glm/glm.hpp"

#include "../assetstore/AssetStore.h"

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
  Tile(int col, int row, int srcCol, int srcRow, int index)
  {
    this->col = col;
    this->row = row;
    this->srcCol = srcCol;
    this->srcRow = srcRow;
    this->index = index;
  }

  int col;
  int row;
  int srcCol;
  int srcRow;
  int index;

  friend std::ostream &operator<<(std::ostream &os, const Tile &t)
  {
    os << t.index;
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
 *    Flood Fi
src/./editor/../gui/../editor/EditorState.h: In constructor ‘Editorll
 *
 *
 */
class TileMapLayer
{
public:
  TileMapLayer(std::string name, std::string tileset, int cols, int rows, int tileSize);
  ~TileMapLayer();

  void render(SDL_Renderer *renderer, SDL_Texture *texture, int xStart, int yStart, double zoom);

  Tile getTile(glm::vec2 position) const;
  void updateTile(glm::vec2 position, glm::vec2 tileData);
  void floodFill(glm::vec2 position, glm::vec2 tileData);

  void clear();
  void initialize(int cols, int rows, int tileSize);

  std::string name;    // Name of the layer (for user)
  std::string tileset; // Name of the tileset (assetId)
  int rows;            // Number of rows
  int cols;            // Number of columns
  int tileSize;        // Size of a side of a tile

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

  std::vector<std::string> tilesets; // list of tilesets used in this tile map

  void setZoom(double zoom);

  void clear();
  void initialize(glm::vec2 mapSize, int tileSize);
  void render(SDL_Renderer *renderer, std::unique_ptr<AssetStore> &assetStore, glm::vec2 position);

  Tile getTile(size_t layer, glm::vec2 position) const;
  void updateTile(size_t layer, glm::vec2 position, glm::vec2 tileData);
  void floodFill(size_t layer, glm::vec2 position, glm::vec2 tileData);

  size_t createLayer(const std::string &name, const std::string &tileset);
  void renameLayer(size_t layerId, const std::string &name);
  void deleteLayer(size_t layerId);
  void moveLayer(size_t layerId, size_t layerIdToSwapWith);
  void swapLayers(size_t layer1, size_t layer2);
  TileMapLayer *getLayer(size_t layerId);
  const std::string &getLayerName(size_t layerId) const;

  size_t layerCount() { return layers.size(); }

private:
  std::vector<TileMapLayer *> layers;
};

#endif