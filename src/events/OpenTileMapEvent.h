#ifndef OPENTILEMAPEVENT_H
#define OPENTILEMAPEVENT_H

#include <string>

#include "../eventbus/Event.h"

struct OpenTileMapEvent : public Event
{
public:
    std::string filepath;
    OpenTileMapEvent(std::string filepath) : filepath(filepath) {}
};

#endif