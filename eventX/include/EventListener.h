#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include <memory>

template <typename T0, typename... T>
class EventListener;

template <typename T>
class EventListener<T>;

template <typename T>
class EventListener<T> {
 public:
  EventListener() = default;
  virtual ~EventListener() = default;

  virtual void onEvent(std::shared_ptr<T>) {}
};

template <typename T0, typename... T>
class EventListener : public EventListener<T...> {
 public:
  EventListener() = default;
  ~EventListener() override = default;

  virtual void onEvent(std::shared_ptr<T0>) {}
};

#endif  // EVENTLISTENER_H
