#ifndef RUNLUAEVENT_H
#define RUNLUAEVENT_H

#include <string>
#include "../eventbus/Event.h"

struct RunLUAEvent : public Event
{
public:
  std::string code;
  RunLUAEvent(std::string code) : code(code) {}
};

#endif