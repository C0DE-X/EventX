#include "EventBus.h"

#include <thread>

EventBus::~EventBus() {
  for (auto dispatcher : m_dispatchers) {
    delete dispatcher.second;
  }
}

void EventBus::push(std::shared_ptr<Event> event) {
  m_eventMutex.lock();
  m_eventQueue.push(event);
  m_eventMutex.unlock();
}

void EventBus::processEvents() {
  m_eventMutex.lock();
  if (!m_eventQueue.empty()) {
    auto event = m_eventQueue.front();
    m_eventQueue.pop();
    m_eventMutex.unlock();
    auto eventType = event->getType();
    for (auto type : eventType) {
      m_dispatcherMutex.lock();
      auto found = m_dispatchers.find(type);
      if (found != m_dispatchers.end()) {
        m_dispatcherMutex.unlock();
        found->second->dispatch(event);
      } else
        m_dispatcherMutex.unlock();
    }
  } else
    m_eventMutex.unlock();
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
