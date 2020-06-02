#ifndef FOOBAR_H
#define FOOBAR_H

#include <EventListener.h>

#include <iostream>

#include "TestEvent.h"

class Foobar : public eventX::EventListener<eventX::Event> {
 public:
  Foobar(eventX::EventBus* bus) : eventX::EventListener<eventX::Event>(bus) {}
  ~Foobar() = default;

  void onEvent(std::shared_ptr<eventX::Event> event) override {
    std::cout << "foobar received event with " << event.get() << std::endl;
  }
};

class Bar : public eventX::EventListener<eventX::Event, TestEvent> {
 public:
  Bar(eventX::EventBus* bus)
      : eventX::EventListener<eventX::Event, TestEvent>(bus) {}
  ~Bar() = default;

  void onEvent(std::shared_ptr<eventX::Event> event) override {
    std::cout << "bar received event with " << event.get() << std::endl;
  }
  void onEvent(std::shared_ptr<TestEvent> event) override {
    std::cout << "bar received testevent with " << event.get() << std::endl;
  }
};

#endif  // FOOBAR_H
