#ifndef TILESET_H
#define TILESET_H

#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../assetstore/AssetStore.h"

class TileSet
{
public:
    TileSet(SDL_Texture *texture, glm::vec2 sizeTile, glm::vec2 sizePixel);
    ~TileSet();

    SDL_Texture *getTexture(std::unique_ptr<AssetStore> &assetStore) const;
    void renderTileAt(SDL_Renderer *renderer, glm::vec2 tileData, glm::vec2 position);

private:
    glm::vec2 sizeTile;  // Size of tileset image in col/row
    glm::vec2 sizePixel; // Size of tileset image in pixels

    SDL_Texture *texture;
};

#endif