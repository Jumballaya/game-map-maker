#ifndef SAVETILEMAPEVENT_H
#define SAVETILEMAPEVENT_H

#include <string>

#include "../eventbus/Event.h"

struct SaveTileMapEvent : public Event
{
public:
    SaveTileMapEvent() {}
};

#endif