#include "interactive_mode.hpp"

#include <cstddef>
#include <iostream>

#include "build_graph.hpp"
#include "builder.hpp"

#include "cli_common.hpp"

void interactiveMode() {
  build_system::Builder builder = constructBuilder(std::cin, &std::cout);

  build_system::BuildGraph build_graph = constructBuildGraph(std::cin, &std::cout);

  size_t target_id = constructTargetId(std::cin, &std::cout);

  execute(builder, build_graph, target_id, &std::cout);

}