#include "EventBus.h"

#include <thread>

namespace eventX {

EventBus::~EventBus() {
  for (auto dispatcher : m_dispatchers.get()) {
    delete dispatcher.second;
  }
}

void EventBus::push(std::shared_ptr<Event> event) {
  m_eventQueue.lock();
  m_eventQueue->push(event);
  m_eventQueue.unlock();
}

void EventBus::processEvents() {
  m_eventQueue.lock();
  if (!m_eventQueue->empty()) {
    auto event = m_eventQueue->front();
    m_eventQueue->pop();
    m_eventQueue.unlock();
    auto eventType = event->getType();
    for (auto type : eventType) {
      m_dispatchers.lock();
      auto found = m_dispatchers->find(type);
      if (found != m_dispatchers->end()) {
        m_dispatchers.unlock();
        found->second->dispatch(event);
      } else
        m_dispatchers.unlock();
    }
  } else
    m_eventQueue.unlock();
}

void EventBus::exec() {
  m_running = true;
  while (m_running) {
    processEvents();
    std::this_thread::yield();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void EventBus::stop() { m_running.store(false); }

}  // namespace eventX
