#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include <EventBus.h>

namespace eventX {

template <typename T0, typename... T>
class EventListener;

template <typename T>
class EventListener<T> : public EventBus::Listener<T> {
 public:
  EventListener(EventBus* bus) : EventBus::Listener<T>(bus) {}
  ~EventListener() override = default;

  virtual void onEvent(std::shared_ptr<T>) override {}
};

template <typename T0, typename... T>
class EventListener : public EventListener<T0>, public EventListener<T...> {
 public:
  EventListener(EventBus* bus)
      : EventListener<T0>(bus), EventListener<T...>(bus) {}
  ~EventListener() override = default;

  void onEvent(std::shared_ptr<T0>) override {}
};

}  // namespace eventX

#endif  // EVENTLISTENER_H
