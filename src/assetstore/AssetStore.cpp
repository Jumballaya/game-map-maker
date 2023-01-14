#include <SDL2/SDL_image.h>

#include "./AssetStore.h"
#include "../logger/Logger.h"

AssetStore::AssetStore()
{
  Logger::Log("[AssetStore] constructor called");
}

AssetStore::~AssetStore()
{
  {
    Logger::Log("[AssetStore] destructor called");
  }
}

void AssetStore::clearAssets()
{
  for (auto texture : textures)
  {
    SDL_DestroyTexture(texture.second);
  }
  textures.clear();
  tilesets.clear();
}

void AssetStore::addTexture(SDL_Renderer *renderer, const std::string &assetId, const std::string &filePath)
{
  SDL_Surface *surface = IMG_Load(filePath.c_str());
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  textures.emplace(assetId, texture);
}

SDL_Texture *AssetStore::getTexture(const std::string &assetId)
{
  return textures[assetId];
}

void AssetStore::addTileset(const std::string &assetId, SDL_Texture *texture, glm::vec2 sizeTile, glm::vec2 sizePixel)
{
  TileSet *ts = new TileSet(texture, sizeTile, sizePixel);
  tilesets.emplace(ts);
}

TileSet *AssetStore::getTileset(const std::string &assetId)
{
  return tilesets[assetId];
}