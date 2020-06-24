#ifndef LOCALEVENTLISTENER_H
#define LOCALEVENTLISTENER_H

#include <EventListener.h>

#include <functional>
#include <type_traits>

namespace eventX {

template <typename T0, typename... T> class LocalEventListener;

template <typename T> class LocalEventListener<T> : public EventListener<T> {
public:
  LocalEventListener(EventBus *bus) : EventListener<T>(bus) {}
  LocalEventListener(EventBus *bus, std::function<void(std::shared_ptr<T>)> cb)
      : EventListener<T>(bus), m_callback(cb) {}
  virtual ~LocalEventListener() = default;

  void setEventCall(std::function<void(std::shared_ptr<T>)> cb) {
    m_callback = cb;
  }

  virtual void onEvent(std::shared_ptr<T> event) override {
    if (m_callback)
      m_callback(event);
  }

private:
  std::function<void(std::shared_ptr<T>)> m_callback;
};

template <typename T0, typename... T>
class LocalEventListener : public EventListener<T0>,
                           public LocalEventListener<T...> {
public:
  LocalEventListener(EventBus *bus) : EventListener<T0>(bus) {}
  ~LocalEventListener() override = default;

  void setEventCall(std::function<void(std::shared_ptr<T0>)> call) {
    m_callback = call;
  }
  using LocalEventListener<T...>::setEventCall;

  virtual void onEvent(std::shared_ptr<T0> event) override {
    if (m_callback)
      m_callback(event);
  }

private:
  std::function<void(std::shared_ptr<T0>)> m_callback;
};

} // namespace eventX

#endif // LOCALEVENTLISTENER_H
