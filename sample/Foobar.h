#ifndef FOOBAR_H
#define FOOBAR_H

#include <EventListener.h>

#include <iostream>

#include "TestEvent.h"

class Foobar : public EventListener<Event> {
 public:
  Foobar(EventBus* bus) : EventListener<Event>(bus) {}
  ~Foobar() = default;

  void onEvent(std::shared_ptr<Event> event) override {
    std::cout << "received event with " << event.get() << std::endl;
  }
};

class Bar : public EventListener<Event, TestEvent> {
 public:
  Bar(EventBus* bus) : EventListener<Event, TestEvent>(bus) {}
  ~Bar() = default;

  void onEvent(std::shared_ptr<Event> event) override {
    std::cout << "received event with " << event.get() << std::endl;
  }
  void onEvent(std::shared_ptr<TestEvent> event) override {
    std::cout << "received testevent with " << event.get() << std::endl;
  }
};

#endif  // FOOBAR_H
