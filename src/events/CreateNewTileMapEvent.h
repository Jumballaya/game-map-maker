#pragma once

#include "../../libs/glm/glm.hpp"
#include "../eventbus/Event.h"

struct CreateNewTileMapEvent : public Event
{
public:
    glm::vec2 sizeInTiles;
    int tileSize;

    CreateNewTileMapEvent(glm::vec2 sizeInTiles, int tileSize) : sizeInTiles(sizeInTiles), tileSize(tileSize){};
};