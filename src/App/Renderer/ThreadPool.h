/*
 * ThreadPool.h
 *
 *  Created on: 11 Dec 2020
 *      Author: Valerio Formato
 * forked from https://gist.github.com/jrandom/ddb986d4e2c6e91926657736e97d2d54
 */

#ifndef RTIAW_utils_threadpool
#define RTIAW_utils_threadpool

// ================================================================================ Standard Includes
// Standard Includes
// --------------------------------------------------------------------------------
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

namespace RTIAW::Utils {
// ============================================================================ Pool
// Pool
//
// A basic thread pool
// ----------------------------------------------------------------------------
class Pool {
public:
  // -------------------------------------------------------------------- Types
  using Mutex_t = std::recursive_mutex;
  using Unique_Lock_t = std::unique_lock<Mutex_t>;
  using Condition_t = std::condition_variable_any;
  using Threads_t = std::vector<std::thread>;

  using Task_Function_t = std::function<void()>;
  using Task_Queue_t = std::queue<Task_Function_t>;

private:
  // -------------------------------------------------------------------- State
  Threads_t m_threads;
  Task_Queue_t m_task_queue;
  Mutex_t m_queue_mutex;
  Condition_t m_pool_notifier;
  bool m_should_stop_processing;
  bool m_is_emergency_stop;
  bool m_is_paused;

public:
  // Construct ( max threads )
  Pool() : Pool(std::thread::hardware_concurrency()) {}

  // Construct ( thread count )
  explicit Pool(const std::size_t thread_count)
      : m_should_stop_processing(false), m_is_emergency_stop(false), m_is_paused(false) {
    // Sanity
    if (thread_count == 0)
      throw std::runtime_error("ERROR: Thread::Pool() -- must have at least one thread");

    // Init pool
    m_threads.reserve(thread_count);

    for (std::size_t i = 0; i < thread_count; ++i)
      m_threads.emplace_back([this]() { Worker(); });
  }

  // Deleted
  Pool(const Pool &source) = delete;
  Pool &operator=(const Pool &source) = delete;

  // Destruct
  ~Pool() {
    // Set stop flag
    {
      Unique_Lock_t queue_lock(m_queue_mutex);
      m_should_stop_processing = true;
    }

    // Wake up all threads and wait for them to exit
    m_pool_notifier.notify_all();

    for (auto &task_thread : m_threads)
      if (task_thread.joinable())
        task_thread.join();
  }

public:
  // ==================================================================== Public API
  // Public API
  // -------------------------------------------------------------------- Accessors
  decltype(m_threads.size()) ThreadCount() const { return m_threads.size(); }

  // -------------------------------------------------------------------- Add_Simple_Task()
  template <typename Lambda_t>

  void AddSimpleTask(Lambda_t &&function) {
    // Add to task queue
    {
      Unique_Lock_t queue_lock(m_queue_mutex);

      // Sanity
      if (m_should_stop_processing || m_is_emergency_stop)
        throw std::runtime_error("ERROR: Thread::Pool::Add_Simple_Task() - attempted to add task to stopped pool");

      // Add our task to the queue
      m_task_queue.emplace(std::forward<Lambda_t>(function));
    }

    // Notify the pool that there is work to do
    m_pool_notifier.notify_one();
  }

  // -------------------------------------------------------------------- Add_Task()
  template <typename Function_t, typename... Args>

  auto AddTask(Function_t &&function, Args &&...args)
      -> std::future<typename std::invoke_result_t<Function_t, Args...>> {
    using return_t = typename std::invoke_result_t<Function_t, Args...>;

    // Create packaged task
    auto task = std::make_shared<std::packaged_task<return_t()>>(
        [Func = std::forward<Function_t>(function), capture0 = std::forward<Args...>(args...)] { return Func(capture0); });

    std::future<return_t> result = task->get_future();

    // Add task to queue
    {
      Unique_Lock_t queue_lock(m_queue_mutex);

      // Sanity
      if (m_should_stop_processing || m_is_emergency_stop)
        throw std::runtime_error("ERROR: Thread::Pool::Add_Task() - attempted to add task to stopped pool");

      // Add our task to the queue
      m_task_queue.emplace([task]() { (*task)(); });
    }

    // Notify the pool that there is work to do
    m_pool_notifier.notify_one();

    return result;
  }

  // -------------------------------------------------------------------- Emergency_Stop()
  void EmergencyStop() {
    {
      Unique_Lock_t queue_lock(m_queue_mutex);
      m_is_emergency_stop = true;
    }

    m_pool_notifier.notify_all();
  }

  // -------------------------------------------------------------------- Pause()
  void Pause(bool pause_state) {
    {
      Unique_Lock_t queue_lock(m_queue_mutex);
      m_is_paused = pause_state;
    }

    m_pool_notifier.notify_all();
  }

  bool IsEmpty() { return m_task_queue.empty(); }

private:
  // ==================================================================== Private API
  // Private API
  // -------------------------------------------------------------------- Worker()
  void Worker() {
    while (true) {
      Task_Function_t task;

      // Scoped waiting / task-retrieval block
      {
        // Wait on tasks or 'stop processing' flags
        Unique_Lock_t queue_lock(m_queue_mutex);

        m_pool_notifier.wait(queue_lock, [this]() {
          return (!m_task_queue.empty() && !m_is_paused) || m_should_stop_processing || m_is_emergency_stop;
        });

        // Bail when stopped and no more tasks remain,
        // or if an emergency stop has been requested.
        if ((m_should_stop_processing && m_task_queue.empty()) || m_is_emergency_stop)
          return;

        // Retrieve next task
        task = std::move(m_task_queue.front());
        m_task_queue.pop();
      }

      // Execute task
      task();
    }
  }
};
} // namespace RTIAW::Utils

#endif
