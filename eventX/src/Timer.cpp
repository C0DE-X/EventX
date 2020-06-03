#include "Timer.h"

namespace eventX {

Timer::~Timer() { stop(); }

void Timer::setTimeout(const std::function<void()> &func,
                       const std::chrono::duration<double> &timeout,
                       int triggercount) {
  stop();
  if (m_res.valid()) m_res.wait();
  m_func = func;
  m_timeout = timeout;
  m_triggerCount = std::max(triggercount, INFINITE);
}

void Timer::start() {
  if (!m_run.exchange(true)) {
    m_res = std::async(std::launch::async, [this]() {
      auto start = std::chrono::steady_clock::now();
      auto counter = m_triggerCount;
      while (m_run) {
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        if (elapsed_seconds >= m_timeout) {
          if (counter == 0) {
            m_run.store(false);
          } else {
            counter = std::max(INFINITE, --counter);
            if (m_func) m_func();
            start = std::chrono::steady_clock::now();
          }
        }
      }
    });
  }
}

void Timer::stop() {
  if (m_run.exchange(false)) {
    m_res.wait();
  }
}

}  // namespace eventX
