#include "thread_pool.hpp"

#include <optional>

#include "task.hpp"

namespace build_system {

void ThreadPool::addTarget(Task&& task, size_t id, 
                           const std::vector<size_t>& dependences) {
  size_t new_id = dependency_manager_.add(id, dependences);
  task_queue_.addIdentTask({std::move(task), new_id});
}

void ThreadPool::start() {
  threads_.reserve(num_threads_);
  for (size_t i = 0; i < num_threads_; ++i) {
    threads_.emplace_back([this]() {
      while (true) {
        auto may_be_task = task_queue_.getIdentTask();
        if (may_be_task == std::nullopt) {
          break;
        }

        dependency_manager_.wait(may_be_task.value().second);

        try {
          may_be_task.value().first();
        } catch (...) {
        }

        dependency_manager_.done(may_be_task.value().second);
      }
    });
  }
}

void ThreadPool::wait() {
  task_queue_.close();
  dependency_manager_.waitAll();

  for (auto& thread : threads_) {
    thread.join();
  }
}

}; // namespace build_system