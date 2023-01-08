#ifndef TILESELECTEVENT_H
#define TILESELECTEVENT_H

#include "../eventbus/Event.h"

struct TileSelectEvent : public Event
{
public:
  int col;
  int row;
  TileSelectEvent(int col, int row) : col(col), row(row){};
};

#endif