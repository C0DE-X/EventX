#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <Event.h>
#include <Lockable.h>

#include <algorithm>
#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <typeindex>

namespace eventX {

class EventBus {
public:
  template <typename EVENT> class Listener : public Lockable<void> {
  public:
    Listener(EventBus *bus) : m_bus(bus) { m_bus->add(this); }
    virtual ~Listener() {
      if (m_bus)
        m_bus->remove(this);
    }

    virtual void onEvent(std::shared_ptr<EVENT> event) = 0;
    void onBusDestroy() { m_bus = nullptr; }

  private:
    EventBus *m_bus;
  };

  EventBus() = default;
  ~EventBus();

  void push(std::shared_ptr<Event> event);
  void processEvent(short maxCount = 1);
  void exec();
  void stop();

protected:
  template <typename EVENT> void add(Listener<EVENT> *l);
  template <typename EVENT> void remove(Listener<EVENT> *l);

private:
  class Dispatcher {
  public:
    Dispatcher() = default;
    virtual ~Dispatcher() = default;
    virtual void dispatch(std::shared_ptr<Event> event) = 0;
  };

  template <typename EVENT> class EventDispatcher : public Dispatcher {
  public:
    EventDispatcher() = default;
    ~EventDispatcher() override {
      for (auto listener : m_listeners.get())
        if (listener)
          listener->onBusDestroy();
    }

    void add(EventBus::Listener<EVENT> *listener);
    void remove(EventBus::Listener<EVENT> *listener);
    void dispatch(std::shared_ptr<Event> event) override;

  private:
    Lockable<std::vector<EventBus::Listener<EVENT> *>> m_listeners;
  };

  std::atomic_bool m_running{false};
  Lockable<std::map<std::type_index, EventBus::Dispatcher *>> m_dispatchers;
  Lockable<std::queue<std::shared_ptr<Event>>> m_eventQueue;
};

/******************************************************************************/

template <typename EVENT> void EventBus::add(EventBus::Listener<EVENT> *l) {
  m_dispatchers.lock();
  if (m_dispatchers->find(std::type_index(typeid(EVENT))) ==
      m_dispatchers->end()) {
    m_dispatchers->insert(std::pair<std::type_index, EventBus::Dispatcher *>(
        std::type_index(typeid(EVENT)),
        new EventBus::EventDispatcher<EVENT>()));
  }
  if (EventBus::EventDispatcher<EVENT> *dispatcher =
          dynamic_cast<EventDispatcher<EVENT> *>(
              m_dispatchers->find(std::type_index(typeid(EVENT)))->second))
    dispatcher->add(l);
  m_dispatchers.unlock();
}

/******************************************************************************/

template <typename EVENT> void EventBus::remove(EventBus::Listener<EVENT> *l) {
  m_dispatchers.lock();
  if (EventBus::EventDispatcher<EVENT> *dispatcher =
          dynamic_cast<EventDispatcher<EVENT> *>(
              m_dispatchers->find(std::type_index(typeid(EVENT)))->second))
    dispatcher->remove(l);
  m_dispatchers.unlock();
}

/******************************************************************************
 * EventDispatcher
 ******************************************************************************/

template <typename EVENT>
void EventBus::EventDispatcher<EVENT>::add(
    EventBus::Listener<EVENT> *listener) {
  m_listeners.lock();
  m_listeners->push_back(listener);
  m_listeners.unlock();
}

/******************************************************************************/
template <typename EVENT>
void EventBus::EventDispatcher<EVENT>::remove(
    EventBus::Listener<EVENT> *listener) {
  m_listeners.lock();
  auto found = std::find(std::begin(m_listeners.get()),
                         std::end(m_listeners.get()), listener);
  if (found != m_listeners->end()) {
    Listener<EVENT> *l = *found;
    l->lock();
    *found = nullptr;
    l->unlock();
  }
  m_listeners.unlock();
}

/******************************************************************************/
template <typename EVENT>
void EventBus::EventDispatcher<EVENT>::dispatch(std::shared_ptr<Event> event) {
  std::vector<EventBus::Listener<EVENT> **> stack;
  m_listeners.lock();
  for (auto &l : m_listeners.get())
    stack.push_back(&l);

  if (auto typeEvent = std::dynamic_pointer_cast<EVENT>(event)) {
    for (auto listener : stack) {
      Listener<EVENT> *l = *listener;
      if (l) {
        l->lock();
        m_listeners.unlock();
        l->onEvent(typeEvent);
        l->unlock();
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        m_listeners.lock();
      }
    }
  }
  m_listeners->erase(
      std::remove(begin(m_listeners.get()), end(m_listeners.get()), nullptr),
      end(m_listeners.get()));
  m_listeners.unlock();
}

} // namespace eventX

#endif // EVENTBUS_H
