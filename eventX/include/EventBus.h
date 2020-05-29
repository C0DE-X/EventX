#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <Event.h>

#include <algorithm>
#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <typeindex>

class EventBus {
  template <typename EVENT>
  friend class Listener;

 public:
  template <typename EVENT>
  class Listener {
   public:
    Listener(EventBus* bus) : m_bus(bus) { m_bus->addListener(this); }
    virtual ~Listener() {
      if (m_bus) m_bus->removeListener(this);
    }

    virtual void onEvent(std::shared_ptr<EVENT> event) = 0;
    void onBusDestroy() { m_bus = nullptr; }

   private:
    EventBus* m_bus;
  };

  EventBus() = default;
  ~EventBus();

  void push(std::shared_ptr<Event> event);
  void processEvents();
  void exec();
  void stop();

 protected:
  template <typename EVENT>
  void addListener(Listener<EVENT>* l);

  template <typename EVENT>
  void removeListener(Listener<EVENT>* l);

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
    ~EventDispatcher() override {
      for (auto listener : m_listeners)
        if (listener) listener->onBusDestroy();
    }

    void addEventListener(EventBus::Listener<EVENT>* listener) {
      m_listenerMutex.lock();
      m_listeners.push_back(listener);
      m_listenerMutex.unlock();
    }
    void removeEventListener(EventBus::Listener<EVENT>* listener) {
      m_listenerMutex.lock();
      auto found =
          std::find(std::begin(m_listeners), std::end(m_listeners), listener);
      if (found != m_listeners.end()) *found = nullptr;
      m_listenerMutex.unlock();
    }
    void dispatch(std::shared_ptr<Event> event) override {
      m_listenerMutex.lock();
      std::vector<EventBus::Listener<EVENT>**> stack;
      for (auto& l : m_listeners) stack.push_back(&l);

      if (auto typeEvent = std::dynamic_pointer_cast<EVENT>(event)) {
        for (auto listener : stack) {
          auto l = *listener;
          m_listenerMutex.unlock();
          if (l) (l)->onEvent(typeEvent);
          m_listenerMutex.lock();
        }
      }
      m_listeners.erase(
          std::remove(begin(m_listeners), end(m_listeners), nullptr),
          end(m_listeners));
      m_listenerMutex.unlock();
    }

   private:
    std::vector<EventBus::Listener<EVENT>*> m_listeners;
    std::recursive_mutex m_listenerMutex;
  };

  std::atomic_bool m_running{false};
  std::map<std::type_index, EventBus::Dispatcher*> m_dispatchers;
  std::queue<std::shared_ptr<Event>> m_eventQueue;
  std::mutex m_dispatcherMutex;
  std::mutex m_eventMutex;
};

template <typename EVENT>
void EventBus::addListener(EventBus::Listener<EVENT>* l) {
  m_dispatcherMutex.lock();
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
  m_dispatcherMutex.unlock();
}

template <typename EVENT>
void EventBus::removeListener(EventBus::Listener<EVENT>* l) {
  m_dispatcherMutex.lock();
  if (EventBus::EventDispatcher<EVENT>* dispatcher =
          dynamic_cast<EventDispatcher<EVENT>*>(
              m_dispatchers.find(std::type_index(typeid(EVENT)))->second))
    dispatcher->removeEventListener(l);
  m_dispatcherMutex.unlock();
}

#endif  // EVENTBUS_H
