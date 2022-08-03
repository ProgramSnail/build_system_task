#pragma once

#include <cstddef>
#include <vector>

#include "task.hpp"

namespace build_system {

struct IncorrectBuildGraph {};

struct Rule {
  size_t target;
  size_t dependency;
};

struct Target {
  size_t id;
  Task task;
  std::vector<size_t> dependences;
};

class BuildGraph {
private:
  // manage DFS, topsort, etc. safe
  class TraversalManager {
  private:
    class View;

  public:
    TraversalManager(const BuildGraph& build_graph)
        : build_graph_(build_graph), epoch_(kNeverVisited + 1) {}

    void initArray() {
      visited_ = std::vector<size_t>(build_graph_.graph_.size(), kNeverVisited);
    }

    View getView();

  private:
    void nextEpoch() { ++epoch_; }

    void DFS(size_t vertex, std::vector<size_t>& current_visited);

  private:
    static const size_t kCurrentWayEpoch;
    static const size_t kNeverVisited;

    const BuildGraph &build_graph_;
    std::vector<size_t> visited_;
    size_t epoch_;
  };

public:
  BuildGraph(const std::vector<Rule>& rules, std::vector<Task>&& tasks);

  void isCorrectOrThrow(size_t target) const;

  bool isCorrect(size_t target) const;

  std::vector<Target> getRequiredTargetsOrder(size_t target) const;

private:
  std::vector<Target> graph_;
  mutable TraversalManager traversal_manager_;
};

} // namespace build_system