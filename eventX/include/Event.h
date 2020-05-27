#ifndef EVENT_H
#define EVENT_H

#include <iterator>
#include <memory>
#include <typeindex>
#include <vector>

class Event {
 public:
  class Type {
   public:
    Type() = default;
    Type(std::type_index const& type);
    Type(Type const& other);
    Type(Type&& other);
    ~Type() = default;

    Type& operator=(Type const& other);
    Type& operator=(Type&& other);
    Type& operator+=(Type const& other);
    Type operator+(Type const& other);

    bool operator==(std::type_index const& type);
    bool operator!=(std::type_index const& type);

    std::vector<std::type_index>::iterator begin();
    std::vector<std::type_index>::iterator end();

    template <typename EVENT>
    static Type fromEvent(EVENT*) {
      return Type(std::type_index(typeid(EVENT)));
    }

   private:
    std::vector<std::type_index> m_types;
  };

  Event() = default;
  virtual ~Event() = default;

  virtual Event::Type getType();
};

#endif  // EVENT_H
