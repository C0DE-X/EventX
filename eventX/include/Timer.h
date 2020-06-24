#ifndef TIMER_H
#define TIMER_H

#include <atomic>
#include <chrono>
#include <functional>
#include <future>

namespace eventX {

class Timer {
 public:
  static constexpr int const& INFINITE = -1;

  Timer() = default;
  ~Timer();

  void setTimeout(std::function<void()> const& func,
                  std::chrono::milliseconds const& timeout,
                  int triggercount = INFINITE);

  void start();
  void stop();

 private:
  std::atomic_bool m_run{false};
  std::function<void()> m_func;
  std::chrono::duration<double> m_timeout{std::chrono::milliseconds(1)};
  int m_triggerCount{-1};
  std::future<void> m_res;
};

}  // namespace eventX

#endif  // TIMER_H
