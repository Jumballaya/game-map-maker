#ifndef TILETOOLSELECTEVENT_H
#define TILETOOLSELECTEVENT_H

#include "../eventbus/Event.h"
#include "../editor/EditorState.h"

struct TileToolSelectEvent : public Event
{
public:
  TileTool selection;

  TileToolSelectEvent(TileTool selection) : selection(selection){};
};

#endif