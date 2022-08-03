#include "cli_common.hpp"

#include <chrono>

#include "test_tasks.hpp"

void print(const std::string& message, std::ostream* out) {
  if (out) {
    *out << message;
  }
}

template <typename T>
void getParam(const std::string& message, T &param, std::istream& in, std::ostream* out) {
  print(message, out);
  in >> param;
}

build_system::Builder constructBuilder(std::istream& in, std::ostream* out) {
  print("Builder constructor\n", out);

  size_t num_threads;
  getParam("num threads: ", num_threads, in, out);

  return build_system::Builder(num_threads);
}

build_system::BuildGraph constructBuildGraph(std::istream& in, std::ostream* out) {
  print("BuildGraph constructor\n", out);

  size_t rules_count = 0;
  getParam("rules count: ", rules_count, in, out);

  std::vector<build_system::Rule> rules;
  rules.reserve(rules_count);

  print("rules (one per line, in format \"target dependency\"):\n", out);
  for (size_t i = 0; i < rules_count; ++i) {
    build_system::Rule rule;
    in >> rule.target >> rule.dependency;

    --rule.target;
    --rule.dependency;

    rules.push_back(rule);
  }

  size_t targets_count = 0;
  getParam("targets count: ", targets_count, in, out);

  std::vector<build_system::Task> tasks;
  tasks.reserve(targets_count);

  print("durations of tasks for targets (in seconds): ", out);
  for (size_t i = 0; i < targets_count; ++i) {
    size_t duration;
    in >> duration;
    
    tasks.push_back(constructTestTask(duration));
  }

  return build_system::BuildGraph(rules, std::move(tasks));
}

size_t constructTargetId(std::istream& in, std::ostream* out) {
  size_t target_id;
  getParam("target id: ", target_id, in, out);

  --target_id;

  return target_id;
}

void execute(build_system::Builder& builder,
             build_system::BuildGraph& build_graph, size_t target_id,
             std::ostream* out) {
  try {
    auto build_start_time = std::chrono::high_resolution_clock::now();

    builder.execute(build_graph, target_id);
    
    auto build_end_time = std::chrono::high_resolution_clock::now();

    auto build_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        build_end_time - build_start_time);

    if (out) {
      *out << "overall time: " << build_time_ms.count() << "ms\n";
    }

  } catch (build_system::IncorrectBuildGraph) {
    print("incorrect build graph\n", out);
  }
}