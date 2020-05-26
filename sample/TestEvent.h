#ifndef TESTEVENT_H
#define TESTEVENT_H

#include <Event.h>
#include <EventBus.h>

class TestEvent : public Event {
 public:
  TestEvent() : Event() {}
  ~TestEvent() override = default;

  virtual std::vector<std::type_index> getType() override {
    auto types = Event::getType();
    types.push_back(
        std::type_index(typeid(std::remove_pointer<typeof(this)>::type)));
    return types;
  }
};

#endif  // TESTEVENT_H
