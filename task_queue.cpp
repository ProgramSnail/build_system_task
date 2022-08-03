#include "task_queue.hpp"

namespace build_system {

bool TaskQueue::addIdentTask(IdentTask&& task) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (is_closed_) {
    return false;
  }
  tasks_.push_back(std::move(task));
  task_exist_.notify_one();
  return true;
}

std::optional<IdentTask> TaskQueue::getIdentTask() {
  std::unique_lock<std::mutex> lock(mutex_);
  while (tasks_.empty() && !is_closed_) {
    task_exist_.wait(lock);
  }
  if (tasks_.empty()) {
    return std::nullopt;
  }

  IdentTask task = tasks_.front();
  tasks_.pop_front();

  return std::move(task);
}

}; // namespace build_system