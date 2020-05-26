#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <Event.h>

#include <atomic>
#include <map>
#include <memory>
#include <queue>
#include <typeindex>

class EventBus {
 public:
  template <typename EVENT>
  class Listener {
   public:
    Listener(EventBus* bus) : m_bus(bus) { bus->addListener(this); }
    virtual ~Listener() { /*remove from dispatcher*/
    }

    virtual void onEvent(std::shared_ptr<EVENT> event) = 0;

   private:
    EventBus* m_bus;
  };

  EventBus() = default;
  ~EventBus() = default;

  template <typename EVENT>
  void addListener(Listener<EVENT>* l);
  void push(std::shared_ptr<Event> event);
  void processEvents();
  void exec();
  void stop();

 private:
  class Dispatcher {
   public:
    Dispatcher() = default;
    virtual ~Dispatcher() = default;
    virtual void dispatch(std::shared_ptr<Event> event) = 0;
  };

  template <typename EVENT>
  class EventDispatcher : public Dispatcher {
   public:
    EventDispatcher() = default;
    ~EventDispatcher() override = default;

    void addEventListener(EventBus::Listener<EVENT>* listener) {
      m_listeners.push_back(listener);
    }
    void dispatch(std::shared_ptr<Event> event) override {
      auto typeEvent = std::dynamic_pointer_cast<EVENT>(event);
      if (typeEvent)
        for (EventBus::Listener<EVENT>* listener : m_listeners)
          listener->onEvent(typeEvent);
    }

   private:
    std::vector<Listener<EVENT>*> m_listeners;
  };

  std::atomic_bool m_running{false};
  std::map<std::type_index, EventBus::Dispatcher*> m_dispatchers;
  std::queue<std::shared_ptr<Event>> m_eventQueue;
};

template <typename EVENT>
void EventBus::addListener(EventBus::Listener<EVENT>* l) {
  if (m_dispatchers.find(std::type_index(typeid(EVENT))) ==
      m_dispatchers.end()) {
    m_dispatchers.insert(std::pair<std::type_index, EventBus::Dispatcher*>(
        std::type_index(typeid(EVENT)),
        new EventBus::EventDispatcher<EVENT>()));
  }
  if (EventBus::EventDispatcher<EVENT>* dispatcher =
          dynamic_cast<EventDispatcher<EVENT>*>(
              m_dispatchers.find(std::type_index(typeid(EVENT)))->second))
    dispatcher->addEventListener(l);
}

#endif  // EVENTBUS_H
