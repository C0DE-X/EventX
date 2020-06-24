#ifndef LOCKABLE_H
#define LOCKABLE_H

#include <mutex>

namespace eventX {

template <typename T> class Lockable {
public:
  template <typename... ARGS> Lockable(ARGS... args) : m_obj(args...) {}
  ~Lockable() = default;

  void lock() { m_lock.lock(); }
  bool tryLock() { return m_lock.try_lock(); }
  void unlock() { m_lock.unlock(); }

  T *operator->() { return &m_obj; }
  T &get() { return m_obj; }

private:
  T m_obj;
  mutable std::recursive_mutex m_lock;
};

template <> class Lockable<void> {
public:
  Lockable() = default;
  ~Lockable() = default;

  void lock() { m_lock.lock(); }
  bool tryLock() { return m_lock.try_lock(); }
  void unlock() { m_lock.unlock(); }

private:
  mutable std::recursive_mutex m_lock;
};

} // namespace eventX

#endif // LOCKABLE_H
