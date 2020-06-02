#ifndef TESTEVENT_H
#define TESTEVENT_H

#include <Event.h>
#include <EventBus.h>

class TestEvent : public eventX::Event {
 public:
  TestEvent() : eventX::Event() {}
  ~TestEvent() override = default;

  eventX::Event::Type getType() override {
    return Type::fromEvent(this) + eventX::Event::getType();
  }
};

#endif  // TESTEVENT_H
