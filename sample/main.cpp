#include <EventX.h>

#include <iostream>

#include "Foobar.h"

int main() {
  std::cout << "Hello World!" << std::endl;

  EventX event;
  Foobar foo;

  auto i = std::make_shared<int>(int(5));
  auto f = std::make_shared<float>(float(43.5f));

  foo.onEvent(i);
  foo.onEvent(f);

  // event.exec();

  return 0;
}
