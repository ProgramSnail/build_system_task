#include "build_graph.hpp"

#include <algorithm>
#include <new>

namespace build_system {

const size_t BuildGraph::TraversalManager::kCurrentWayEpoch = 0;
const size_t BuildGraph::TraversalManager::kNeverVisited = 1;

class BuildGraph::TraversalManager::View {
  friend TraversalManager;

public:
  View() = delete;

  std::vector<size_t> DFS(size_t vertex) {
    std::vector<size_t> visited;
    traversal_manager.DFS(vertex, visited);
    return visited;
  }

  // this function saves exsisting values in "visited" array
  void DFS(size_t vertex, std::vector<size_t>& visited) {
    traversal_manager.DFS(vertex, visited);
  }

  virtual ~View() { traversal_manager.nextEpoch(); }

private:
  View(TraversalManager &traversal_manager)
      : traversal_manager(traversal_manager) {}

private:
  TraversalManager &traversal_manager;
};

BuildGraph::TraversalManager::View BuildGraph::TraversalManager::getView() {
  return View(*this);
}

// this function saves existing values in "visited" array
void BuildGraph::TraversalManager::DFS(size_t vertex,
                                       std::vector<size_t>& current_visited) {
  if (visited_[vertex] == epoch_) {
    return;
  }

  if (visited_[vertex] == kCurrentWayEpoch) {
    throw IncorrectBuildGraph{};
  }

  visited_[vertex] = kCurrentWayEpoch;

  for (auto &dependency : build_graph_.graph_[vertex].dependences) {
    DFS(dependency, current_visited);
  }

  visited_[vertex] = epoch_;

  current_visited.push_back(vertex);
}

BuildGraph::BuildGraph(const std::vector<Rule>& rules, 
                       std::vector<Task>&& tasks)
    : traversal_manager_(*this) {
  graph_.reserve(tasks.size());

  for (size_t i = 0; i < tasks.size(); ++i) {
    Target target;
    target.task = std::forward<Task>(tasks[i]);
    target.id = i;
    graph_.push_back(target);
  }

  for (auto &rule : rules) {
    graph_[rule.target].dependences.push_back(rule.dependency);
  }

  traversal_manager_.initArray();
}

void BuildGraph::isCorrectOrThrow(size_t target) const {
  traversal_manager_.getView().DFS(target);
}

bool BuildGraph::isCorrect(size_t target) const {
  try {
    isCorrectOrThrow(target);
  } catch (IncorrectBuildGraph) {
    return false;
  }
  return true;
}

std::vector<Target> BuildGraph::getRequiredTargetsOrder(size_t target) const {

  std::vector<Target> order;

  auto rev_topsorted_required_verticles = traversal_manager_.getView().DFS(target);

  order.reserve(rev_topsorted_required_verticles.size());
  for (auto &vertex_id : rev_topsorted_required_verticles) {
    order.push_back(graph_[vertex_id]);
  }

  return order;
}

} // namespace build_system