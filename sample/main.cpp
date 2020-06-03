#include <EventBus.h>
#include <LocalEventListener.h>
#include <Timer.h>

#include <iostream>
#include <thread>

#include "Foobar.h"
#include "TestEvent.h"

int main() {
  std::cout << "Start...\n" << std::endl;

  eventX::EventBus eventBus;
  eventX::EventBus eventBus1;
  Foobar foo(&eventBus);
  Bar bar(&eventBus1);
  eventX::LocalEventListener<TestEvent> ll(&eventBus1);
  eventX::Timer timer;

  ll.setEventCall([](std::shared_ptr<TestEvent> event) {
    std::cout << "locally testevent received " << event.get() << " in "
              << std::this_thread::get_id() << std::endl;
  });

  timer.setTimeout(
      [&eventBus]() {
        std::cout << "Timmer got triggered!\n";
        eventBus.push(std::make_shared<TestEvent>());
      },
      std::chrono::seconds(3), 5);

  std::thread t([&eventBus, &eventBus1]() {
    std::cout << "Press ENTER to stop\n";
    std::cin.ignore();
    eventBus.stop();
    eventBus1.stop();
  });

  timer.start();

  std::thread et([&eventBus1]() { eventBus1.exec(); });

  eventBus.push(std::make_shared<TestEvent>());
  eventBus.push(std::make_shared<eventX::Event>());
  eventBus1.push(std::make_shared<eventX::Event>());
  eventBus1.push(std::make_shared<TestEvent>());
  eventBus.exec();

  t.join();
  et.join();
  std::cout << "...end\n";

  return 0;
}
