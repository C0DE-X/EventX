#include "Event.h"

std::vector<std::type_index> Event::getType() {
  return {std::type_index(typeid(std::remove_pointer<typeof(this)>::type))};
}
