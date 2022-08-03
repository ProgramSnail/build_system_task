#include "dependency_manager.hpp"

namespace build_system {

void DependencyManager::waitAll() {
  for (size_t i = 0; i < dependency_counters_.size(); ++i) {
    wait(i);
  }
}

void DependencyManager::wait(size_t id) {
  auto &counter = dependency_counters_[id];
  size_t counter_value = counter.load();
  while (counter_value != 0) {
    counter.wait(counter_value);
    counter_value = counter.load();
  }
}

size_t DependencyManager::add(size_t id,
                              const std::vector<size_t>& dependences) {
  size_t new_id = dependency_counters_.size();

  dependency_counters_.emplace_back(dependences.size());

  tasks_consequences_.emplace_back();

  for (auto& dependency_id : dependences) {
    tasks_consequences_[to_new_id[dependency_id]].push_back(new_id);
  }

  to_new_id[id] = new_id;
  return new_id;
}

void DependencyManager::done(size_t id) {
  for (auto &consequence_id : tasks_consequences_[id]) {
    if (dependency_counters_[consequence_id].fetch_sub(1) == 1) {
      dependency_counters_[consequence_id].notify_all();
    }
  }
}

}; // namespace build_system