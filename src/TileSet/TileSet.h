#ifndef TILESET_H
#define TILESET_H

#include <string>
#include <memory>

#include "../../libs/glm/glm.hpp"
#include <SDL2/SDL.h>

class TileSet
{
public:
    glm::vec2 sizeTile;   // Size of tileset image in col/row
    glm::vec2 sizePixel;  // Size of tileset image in pixels
    int tileSize;         // Size of a size of an individual tile in pixels
    std::string filePath; // Filepath for saving the map file

    TileSet(SDL_Renderer *renderer, const std::string &filePath, glm::vec2 sizeTile, glm::vec2 sizePixel, int tileSize);
    ~TileSet();

    SDL_Texture *getTexture() const;
    void renderTileAt(SDL_Renderer *renderer, glm::vec2 tileData, glm::vec2 position);

private:
    SDL_Texture *texture;
};

#endif