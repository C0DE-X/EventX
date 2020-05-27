#ifndef TESTEVENT_H
#define TESTEVENT_H

#include <Event.h>
#include <EventBus.h>

class TestEvent : public Event {
 public:
  TestEvent() : Event() {}
  ~TestEvent() override = default;

  Event::Type getType() override {
    return Type::fromEvent(this) + Event::getType();
  }
};

#endif  // TESTEVENT_H
