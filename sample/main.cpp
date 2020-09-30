#include <iostream>
#include <thread>

#include <EventBus.h>
#include <LocalEventListener.h>
#include <Timer.h>

class TestEvent : public eventX::Event {
public:
  TestEvent() : eventX::Event() {}
  ~TestEvent() override = default;

  EVENTTYPE(TestEvent, eventX::Event)
};

class Testlistener1 : public eventX::EventListener<eventX::Event> {
public:
  Testlistener1(eventX::EventBus *bus)
      : eventX::EventListener<eventX::Event>(bus) {}
  ~Testlistener1() = default;

  void onEvent(std::shared_ptr<eventX::Event> event) override {
    std::cout << "foobar received event with " << event.get() << " in "
              << std::this_thread::get_id() << std::endl;
  }
};

class Testlistener2 : public eventX::EventListener<eventX::Event, TestEvent> {
public:
  Testlistener2(eventX::EventBus *bus)
      : eventX::EventListener<eventX::Event, TestEvent>(bus) {}
  ~Testlistener2() = default;

  void onEvent(std::shared_ptr<eventX::Event> event) override {
    std::cout << "bar received event with " << event.get() << " in "
              << std::this_thread::get_id() << std::endl;
  }
  void onEvent(std::shared_ptr<TestEvent> event) override {
    std::cout << "bar received testevent with " << event.get() << " in "
              << std::this_thread::get_id() << std::endl;
  }
};

int main() {
  std::cout << "Start...\n" << std::endl;

  eventX::EventBus eventBus;
  eventX::EventBus eventBus1;

  Testlistener1 l1(&eventBus);
  Testlistener2 l2(&eventBus1);

  eventX::LocalEventListener<TestEvent> ll(&eventBus1);
  eventX::Timer timer;

  ll.setEventCall([](std::shared_ptr<TestEvent> event) {
    std::cout << "locally testevent received " << event.get() << " in "
              << std::this_thread::get_id() << std::endl;
  });

  timer.setTimeout(
      [&eventBus]() {
        std::cout << "Timer got triggered!\n";
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

  eventBus.push(std::make_shared<eventX::Event>());
  eventBus.push(std::make_shared<TestEvent>());
  eventBus1.push(std::make_shared<eventX::Event>());
  eventBus1.push(std::make_shared<TestEvent>());
  eventBus.exec();

  t.join();
  et.join();
  std::cout << "...end\n";

  return 0;
}
