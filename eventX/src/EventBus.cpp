#include "EventBus.h"

#include <thread>

void EventBus::push(std::shared_ptr<Event> event) { m_eventQueue.push(event); }

void EventBus::processEvents() {
  if (!m_eventQueue.empty()) {
    auto event = m_eventQueue.front();
    m_eventQueue.pop();
    auto eventType = event->getType();
    for (auto type : eventType) {
      auto found = m_dispatchers.find(type);
      if (found != m_dispatchers.end()) {
        found->second->dispatch(event);
      }
    }
  }
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
