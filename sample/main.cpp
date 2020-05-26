#include <EventBus.h>
#include <LocalEventListener.h>

#include <iostream>
#include <thread>

#include "Foobar.h"
#include "TestEvent.h"

int main() {
  std::cout << "Start...\n" << std::endl;

  EventBus eventBus;
  Foobar foo(&eventBus);
  Bar bar(&eventBus);
  LocalEventListener<TestEvent> ll(&eventBus);
  ll.setEventCall([](std::shared_ptr<TestEvent> event) {
    std::cout << "locally testevent received " << event << std::endl;
  });

  std::thread t([&eventBus]() {
    std::cout << "Press ENTER to stop\n";
    std::cin.ignore();
    eventBus.stop();
  });

  eventBus.push(std::make_shared<TestEvent>());
  eventBus.push(std::make_shared<Event>());
  eventBus.exec();

  t.join();
  std::cout << "...end\n";

  return 0;
}
