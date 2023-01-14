#include "./TileSet.h"

TileSet::TileSet(SDL_Texture *texture, glm::vec2 sizeTile, glm::vec2 sizePixel)
{
    this->texture = texture;
    this->sizeTile = sizeTile;
    this->sizePixel = sizePixel;
};

TileSet::~TileSet()
{
    SDL_DestroyTexture(texture);
}

SDL_Texture *TileSet::getTexture(std::unique_ptr<AssetStore> &assetStore) const
{
    return texture;
};
