#pragma once

#include <cstddef>
#include <functional>
#include <thread>
#include <vector>

#include "dependency_manager.hpp"
#include "task_queue.hpp"

namespace build_system {

class ThreadPool {
public:
  explicit ThreadPool(size_t num_threads) : num_threads_(num_threads) {}

  void addTarget(Task&& task, size_t id, const std::vector<size_t>& dependences);

  void start();

  void wait();

private:
  size_t num_threads_;
  std::vector<std::thread> threads_;
  TaskQueue task_queue_;
  DependencyManager dependency_manager_;
};

}; // namespace build_system