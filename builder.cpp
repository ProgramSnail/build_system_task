#include "builder.hpp"

namespace build_system {

void Builder::execute(const BuildGraph& build_graph, size_t target_id) {
  // can throw IncorrectBuildGraph
  auto targets_order = build_graph.getRequiredTargetsOrder(target_id);

  for (auto& target : targets_order) {
    thread_pool_.addTarget(std::move(target.task), target.id, target.dependences);
  }

  thread_pool_.start();

  thread_pool_.wait();
}

}; // namespace build_system