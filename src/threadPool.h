#pragma once
#include <functional>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

class ThreadPool
{
  unsigned m_threads_count;
  std::vector<std::thread> m_threads;
  std::vector<std::function<void(void)>> m_tasks;
  std::condition_variable m_condition_variable;
  std::mutex m_waiting_mutex;
  std::mutex m_tasks_mutex;
  std::mutex m_log_mutex;
  std::atomic<bool> m_is_working;
  std::atomic<unsigned> m_working_threads_count;

  void ThreadCycle(unsigned i_thread_id);
  void ThreadSafeLog(std::string);

 public:
  ThreadPool(unsigned i_threads_count = 0);
  ~ThreadPool();

  template <typename T>
  std::future<T> Do(std::function<T(void)> i_func);
};

template <typename T>
std::future<T> ThreadPool::Do(std::function<T(void)> i_func)
{
  std::shared_ptr<std::promise<T>> result_accumulator =
      std::make_shared<std::promise<T>>();
  {
    std::lock_guard<std::mutex> lock(m_tasks_mutex);
    m_tasks.emplace_back([result_accumulator, &i_func] {
      result_accumulator->set_value(i_func());
    });
  }
  std::future<T> result = result_accumulator->get_future();
  m_condition_variable.notify_one();
  ThreadSafeLog("notified one");
  return result;
}
