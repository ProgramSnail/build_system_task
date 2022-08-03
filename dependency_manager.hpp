#pragma once

#include <atomic>
#include <cstddef>
#include <deque>
#include <unordered_map>
#include <vector>

namespace build_system {

class DependencyManager {
public:
  DependencyManager() {}

  void waitAll(); // async

  void wait(size_t id); // async

  size_t add(size_t id, const std::vector<size_t>& dependences); // sync

  void done(size_t id); // async

private:
  std::deque<std::atomic<size_t>> dependency_counters_;
  std::vector<std::vector<size_t>> tasks_consequences_;
  std::unordered_map<size_t, size_t> to_new_id;
};

}; // namespace build_system