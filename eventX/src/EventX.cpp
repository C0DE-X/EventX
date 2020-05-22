#include <EventX.h>

EventX &EventX::instance() {
  static EventX x;
  return x;
}

EventBus *EventX::eventBus() { return &m_eventBus; }
