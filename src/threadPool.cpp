#include "threadPool.h"
#include <algorithm>
#include <iostream>
#include <thread>

void ThreadPool::ThreadCycle(unsigned i_thread_id)
{
  std::unique_lock<std::mutex> lk(m_waiting_mutex);
  std::function<void()> func;
  bool tasks_is_empty = true;
  while (m_is_working)
    {
      ThreadSafeLog("thread[" + std::to_string(i_thread_id) + "] waiting");
      {
        std::lock_guard<std::mutex> lock(m_tasks_mutex);
        tasks_is_empty = m_tasks.empty();
      }
      if (tasks_is_empty && m_is_working)
        {
          m_condition_variable.wait(lk);
        }

      // work stage
      m_working_threads_count++;
      if (!m_is_working)
        {
          return;
        }

      {
        std::lock_guard<std::mutex> lock(m_tasks_mutex);
        ThreadSafeLog("thread[" + std::to_string(i_thread_id) +
                      "] start working");
        tasks_is_empty = m_tasks.empty();
        if (!tasks_is_empty)
          {
            func = m_tasks.back();
            m_tasks.pop_back();
          }
      }
      if (!tasks_is_empty)
        {
          func();
          tasks_is_empty = true;
        }

      ThreadSafeLog("thread[" + std::to_string(i_thread_id) +
                    "] complete his task");
      m_working_threads_count--;
    }
}

void ThreadPool::ThreadSafeLog(std::string msg)
{
  std::lock_guard<std::mutex> lock(m_log_mutex);
  std::cout << msg << std::endl;
}

ThreadPool::ThreadPool(unsigned i_threads_count)
    : m_is_working{true}, m_working_threads_count{0}
{
  m_threads_count = 4 /*i_threads_count == 0 ?
                         std::thread::hardware_concurrency() : i_threads_count*/
      ;
  ThreadSafeLog("thread pool created with " + std::to_string(m_threads_count) +
                " threads");
  for (auto i = 0u; i < m_threads_count; i++)
    {
      m_threads.emplace_back(&ThreadPool::ThreadCycle, this, i);
    }
}

ThreadPool::~ThreadPool()
{
  std::cout << "preparing to destroying" << std::endl;
  m_is_working = false;
  m_condition_variable.notify_all();
  for (auto i = 0u; i < m_threads_count; i++)
    {
      m_threads[i].join();
      ThreadSafeLog("thread[" + std::to_string(i) + "] leaved us");
    }
}
