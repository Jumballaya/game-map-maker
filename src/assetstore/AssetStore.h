#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "../TileSet/TileSet.h"

class AssetStore
{
public:
  AssetStore();
  ~AssetStore();

  void clearAssets();

  // Textures
  void addTexture(SDL_Renderer *renderer, const std::string &assetId, const std::string &filePath);
  SDL_Texture *getTexture(const std::string &assetId);

  // Fonts

  // TileSets
  // void addTileset(const std::string &assetId, const std::string &filePath); // Set this up when we XML loading working
  void addTileset(const std::string &assetId, SDL_Texture *texture, glm::vec2 sizeTile, glm::vec2 sizePixel, int tileSize);
  TileSet *getTileset(const std::string &assetId);

private:
  std::map<std::string, SDL_Texture *> textures;
  std::map<std::string, TTF_Font *> fonts;
  std::map<std::string, TileSet *> tilesets;

  // std::map<std::string, Mix_Chunk *> sounds;
};

#endif