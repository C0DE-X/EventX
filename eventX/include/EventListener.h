#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include <memory>

template <typename T>
class EventListener {
 public:
  EventListener() {}
  virtual ~EventListener() = default;

  virtual void onEvent(std::shared_ptr<T>) {}
};

#endif  // EVENTLISTENER_H
