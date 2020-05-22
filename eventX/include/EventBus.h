#ifndef EVENTBUS_H
#define EVENTBUS_H

class EventBus {
 public:
  EventBus() = default;
  ~EventBus() = default;

  void processEvents();
  void exec();
};

#endif  // EVENTBUS_H
