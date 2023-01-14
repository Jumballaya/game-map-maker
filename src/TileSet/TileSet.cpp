#include "./TileSet.h"

TileSet::TileSet(SDL_Texture *texture, glm::vec2 sizeTile, glm::vec2 sizePixel, int tileSize)
{
    this->texture = texture;
    this->sizeTile = sizeTile;
    this->sizePixel = sizePixel;
    this->tileSize = tileSize;
};

TileSet::~TileSet()
{
    SDL_DestroyTexture(texture);
}

SDL_Texture *TileSet::getTexture() const
{
    return texture;
};
