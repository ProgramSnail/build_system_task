#pragma once

#include <cstddef>

#include "build_graph.hpp"
#include "thread_pool.hpp"

namespace build_system {

class Builder {
public:
  explicit Builder(size_t num_threads) : thread_pool_(num_threads) {}

  void execute(const BuildGraph& build_graph, size_t target_id);

private:
  ThreadPool thread_pool_;
};

}; // namespace build_system