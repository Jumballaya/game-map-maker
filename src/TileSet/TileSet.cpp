#include "./TileSet.h"

#include <SDL2/SDL_image.h>

TileSet::TileSet(SDL_Renderer *renderer, const std::string &filePath, glm::vec2 sizeTile, glm::vec2 sizePixel, int tileSize)
{
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    this->filePath = filePath;
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
