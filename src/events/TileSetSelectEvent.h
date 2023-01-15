#ifndef TILESETSELECTEVENT_H
#define TILESETSELECTEVENT_H

#include <string>
#include "../eventbus/Event.h"

struct TileSetSelectEvent : public Event
{
public:
    std::string tileset;

    TileSetSelectEvent(const std::string &tileset) : tileset(tileset) {}
};

#endif