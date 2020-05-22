#include <EventX.h>
#include <LocalEventListener.h>

#include <iostream>

#include "Foobar.h"

int main() {
  std::cout << "Hello World!" << std::endl;

  Foobar foo;
  LocalEventListener<int, float> ll;
  ll.setEventCall([](std::shared_ptr<int> i) {
    std::cout << "local catch int " << *i << std::endl;
  });
  ll.setEventCall([](std::shared_ptr<float> i) {
    std::cout << "local catch float " << *i << std::endl;
  });

  EventListener<int>* eli = &foo;
  EventListener<float>* elf = &foo;

  auto i = std::make_shared<int>(int(5));
  auto f = std::make_shared<float>(float(43.5f));

  eli->onEvent(i);
  elf->onEvent(f);

  eli = &ll;
  elf = &ll;

  eli->onEvent(i);
  elf->onEvent(f);

  return 0;
}
