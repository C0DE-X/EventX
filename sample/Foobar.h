#ifndef FOOBAR_H
#define FOOBAR_H

#include <EventListener.h>

#include <iostream>

class Foobar : public EventListener<int, float> {
 public:
  Foobar() = default;
  ~Foobar() = default;

  void onEvent(std::shared_ptr<int> event) override {
    std::cout << "received int event with " << *event << std::endl;
  }

  void onEvent(std::shared_ptr<float> event) override {
    std::cout << "received float event with " << *event << std::endl;
  }
};

#endif  // FOOBAR_H
