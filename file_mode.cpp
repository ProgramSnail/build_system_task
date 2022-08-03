#include "file_mode.hpp"

#include <cstddef>
#include <iostream>
#include <fstream>

#include "build_graph.hpp"
#include "builder.hpp"

#include "cli_common.hpp"

void fileMode(const std::string& filename) {
  std::ifstream in;
  in.open(filename);

  build_system::Builder builder = constructBuilder(in);

  build_system::BuildGraph build_graph = constructBuildGraph(in);

  size_t target_id = constructTargetId(in);

  execute(builder, build_graph, target_id, &std::cout);

  in.close();
}
