#ifndef KEYPRESSEVENT_H
#define KEYPRESSEVENT_H

#include <SDL2/SDL.h>

class KeyPressEvent : public Event
{
public:
  SDL_Keycode symbol;
  KeyPressEvent(SDL_Keycode symbol) : symbol(symbol) {}
};

#endif