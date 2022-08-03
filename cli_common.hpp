#pragma once

#include <cstddef>
#include <iostream>
#include <optional>

#include "build_graph.hpp"
#include "builder.hpp"

void print(const std::string& message, std::ostream* out);

template <typename T>
void getParam(const std::string& message, T& param, std::istream &in,
              std::ostream* out);

build_system::Builder constructBuilder(std::istream& in,
                                       std::ostream* out = nullptr);

build_system::BuildGraph constructBuildGraph(std::istream& in, 
                                             std::ostream* out = nullptr);

size_t constructTargetId(std::istream& in, std::ostream* out = nullptr);

void execute(build_system::Builder& builder,
             build_system::BuildGraph& build_graph, size_t target_id,
             std::ostream* out = nullptr);