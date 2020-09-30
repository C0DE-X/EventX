#ifndef EVENT_H
#define EVENT_H

#include <iterator>
#include <memory>
#include <typeindex>
#include <vector>

#define TYPE_MACRO(_1, _2, _3, _4, NAME, ...) NAME
#define EVENTTYPE(...) TYPE_MACRO(__VA_ARGS__, TYPE4, TYPE3, TYPE2)(__VA_ARGS__)

#define TYPE2(CLASS, BASE)                                                     \
  Type getType() override {                                                    \
    static Type type = Type(typeid(CLASS)) + BASE::getType();                  \
    return type;                                                               \
  }

#define TYPE3(CLASS, BASE, BASE1)                                              \
  Type getType() override {                                                    \
    static Type type =                                                         \
        Type(typeid(CLASS)) + BASE::getType() + BASE1::getType();              \
    return type;                                                               \
  }

#define TYPE4(CLASS, BASE, BASE1, BASE2)                                       \
  Type getType() override {                                                    \
    static Type type = Type(typeid(CLASS)) + BASE::getType() +                 \
                       BASE1::getType() + BASE2::getType();                    \
    return type;                                                               \
  }

namespace eventX {

class Event {
public:
  class Type {
  public:
    Type() = default;
    Type(std::type_index const &type);
    Type(Type const &other);
    Type(Type &&other);
    ~Type() = default;

    Type &operator=(Type const &other);
    Type &operator=(Type &&other);
    Type &operator+=(Type const &other);
    Type operator+(Type const &other);

    bool operator==(std::type_index const &type);
    bool operator!=(std::type_index const &type);

    std::vector<std::type_index>::iterator begin();
    std::vector<std::type_index>::iterator end();

    template <typename EVENT> static Type fromEvent(EVENT *) {
      return Type(std::type_index(typeid(EVENT)));
    }

  private:
    std::vector<std::type_index> m_types;
  };

  Event(std::string const &id = std::string());
  virtual ~Event() = default;

  virtual Event::Type getType();
  std::string getId() const;

private:
  std::string m_id;
};

} // namespace eventX

#endif // EVENT_H
