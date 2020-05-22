#ifndef EVENTX_H
#define EVENTX_H

#include <EventBus.h>
#include <EventX_globals.h>

class EVENTX_EXPORT EventX {
 public:
  static EventX& instance();
  ~EventX() = default;

  EventBus* eventBus();

 private:
  EventX() = default;
  EventBus m_eventBus;
};

#endif  // EVENTX_H
