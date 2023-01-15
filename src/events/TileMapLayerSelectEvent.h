#ifndef TILEMAPLAYERSELECTEVENT_H
#define TILEMAPLAYERSELECTEVENT_H

#include <memory>

#include "../eventbus/Event.h"

struct TileMapLayerSelectEvent : public Event
{
public:
    size_t layerId;
    TileMapLayerSelectEvent(size_t layerId) : layerId(layerId) {}
};

#endif