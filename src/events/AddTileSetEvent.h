#pragma once

#include <string>

#include "../../libs/glm/glm.hpp"
#include "../eventbus/Event.h"

class AddTileSetEvent : public Event
{
public:
    std::string filePath;
    std::string assetId;
    glm::vec2 sizeInTiles;
    int tileSize;

    AddTileSetEvent(
        const std::string &assetId,
        const std::string &filePath,
        glm::vec2 sizeInTiles,
        int tileSize)
    {
        this->filePath = filePath;
        this->assetId = assetId;
        this->sizeInTiles = sizeInTiles;
        this->tileSize = tileSize;
    };
};