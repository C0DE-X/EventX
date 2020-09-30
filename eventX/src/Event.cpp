#include "Event.h"

#include <algorithm>

namespace eventX {

Event::Event(const std::string &id) : m_id(id) {}

Event::Type Event::getType() { return Type::fromEvent(this); }

std::string Event::getId() const { return m_id; }

Event::Type::Type(const std::type_index &type) : m_types({type}) {}

Event::Type::Type(const Event::Type &other) : m_types(other.m_types) {}

Event::Type::Type(Event::Type &&other) : m_types(std::move(other.m_types)) {}

Event::Type &Event::Type::operator=(const Event::Type &other) {
  m_types = other.m_types;
  return *this;
}

Event::Type &Event::Type::operator=(Event::Type &&other) {
  m_types = std::move(other.m_types);
  return *this;
}

Event::Type &Event::Type::operator+=(const Event::Type &other) {
  m_types.insert(m_types.end(), other.m_types.begin(), other.m_types.end());
  std::sort(m_types.begin(), m_types.end());
  m_types.erase(std::unique(m_types.begin(), m_types.end()), m_types.end());
  return *this;
}

Event::Type Event::Type::operator+(const Event::Type &other) {
  return Event::Type(*this) += other;
}

bool Event::Type::operator==(const std::type_index &type) {
  return std::find_if(m_types.begin(), m_types.end(),
                      [&type](std::type_index const &t) {
                        return type == t;
                      }) != m_types.end();
}

bool Event::Type::operator!=(const std::type_index &type) {
  return std::find_if(m_types.begin(), m_types.end(),
                      [&type](std::type_index const &t) {
                        return type == t;
                      }) == m_types.end();
}

std::vector<std::type_index>::iterator Event::Type::begin() {
  return m_types.begin();
}

std::vector<std::type_index>::iterator Event::Type::end() {
  return m_types.end();
}

} // namespace eventX
