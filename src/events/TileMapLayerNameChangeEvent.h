#ifndef TILEMAPLAYERNAMECHANGEEVENT_H
#define TILEMAPLAYERNAMECHANGEEVENT_H

#include <string>
#include <memory>

#include "../eventbus/Event.h"

struct TileMapLayerNameChangeEvent : public Event
{
public:
    std::string newName;
    size_t layerId;

    TileMapLayerNameChangeEvent(size_t layerId, const std::string &newName) : newName(newName), layerId(layerId) {}
};

#endif