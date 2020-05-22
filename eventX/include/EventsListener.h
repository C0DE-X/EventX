#ifndef EVENTSLISTENER_H
#define EVENTSLISTENER_H

#include <EventListener.h>

template <typename T0, typename... T>
class EventsListener;

template <typename T>
class EventsListener<T> : public EventListener<T> {
 public:
  EventsListener() : EventListener<T>() {}
  ~EventsListener() override = default;

  virtual void onEvent(std::shared_ptr<T>) override {}
};

template <typename T0, typename... T>
class EventsListener : public EventListener<T0>, public EventsListener<T...> {
 public:
  EventsListener() : EventListener<T0>() {}
  ~EventsListener() override = default;

  virtual void onEvent(std::shared_ptr<T0>) override {}
};

#endif  // EVENTSLISTENER_H
