#ifndef SAVETILEMAPEVENT_H
#define SAVETILEMAPEVENT_H

#include <string>

#include "../eventbus/Event.h"

struct SaveTileMapEvent : public Event
{
public:
    std::string filepath;
    SaveTileMapEvent(std::string filepath) : filepath(filepath) {}
};

#endif