#ifndef EVENT_H
#define EVENT_H

#include <memory>
#include <typeindex>
#include <vector>

class Event {
 public:
  Event() = default;
  virtual ~Event() = default;

  virtual std::vector<std::type_index> getType();
};

#endif  // EVENT_H
