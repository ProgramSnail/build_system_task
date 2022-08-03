#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>

#include "task.hpp"

namespace build_system {

class TaskQueue {
public:
  TaskQueue() : is_closed_(false) {}

  bool addIdentTask(IdentTask&& task);

  std::optional<IdentTask> getIdentTask();

  void close() {
    std::lock_guard<std::mutex> lock(mutex_);
    is_closed_ = true;
    task_exist_.notify_all();
  }

private:
  std::deque<IdentTask> tasks_;
  bool is_closed_;
  std::mutex mutex_;
  std::condition_variable task_exist_;
};

}; // namespace build_system