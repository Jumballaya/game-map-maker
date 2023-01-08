#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <map>
#include <typeindex>
#include <list>
#include <functional>
#include "../logger/Logger.h"
#include "./Event.h"

class IEventCallback
{
public:
  virtual ~IEventCallback() = default;
  void execute(Event &e) { call(e); }

private:
  virtual void call(Event &e) = 0;
};

template <typename Owner, typename E>
class EventCallback : public IEventCallback
{
  typedef void (Owner::*CallbackFunction)(E &);

public:
  EventCallback(Owner *ownerInstance, CallbackFunction callbackFunction)
  {
    this->ownerInstance = ownerInstance;
    this->callbackFunction = callbackFunction;
  }

  virtual ~EventCallback() override = default;

private:
  Owner *ownerInstance;
  CallbackFunction callbackFunction;

  virtual void call(Event &e) override
  {
    std::invoke(callbackFunction, ownerInstance, static_cast<E &>(e));
  }
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus
{
public:
  EventBus()
  {
    Logger::Log("[EventBus] constructor called");
  }

  ~EventBus()
  {
    Logger::Log("[EventBus] destructor called");
  }

  template <typename E, typename Owner>
  void subscribe(Owner *ownerInstance, void (Owner::*callbackFunction)(E &))
  {
    if (!subscribers[typeid(E)].get())
    {
      subscribers[typeid(E)] = std::make_unique<HandlerList>();
    }
    auto subscriber = std::make_unique<EventCallback<Owner, E>>(ownerInstance, callbackFunction);
    subscribers[typeid(E)]->push_back(std::move(subscriber));
  };

  template <typename E, typename... EArgs>
  void emit(EArgs &&...args)
  {
    auto handlers = subscribers[typeid(E)].get();
    if (handlers)
    {
      for (auto it = handlers->begin(); it != handlers->end(); it++)
      {
        auto handler = it->get();
        E event(std::forward<EArgs>(args)...);
        handler->execute(event);
      }
    }
  };

  void reset()
  {
    subscribers.clear();
  }

private:
  std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;
};

#endif