#include <chrono>
#include <cstddef>
#include <thread>

#include <iostream>

#include "task.hpp"

inline build_system::Task constructTestTask(size_t duration) {
  return [duration]() {
    std::this_thread::sleep_for(std::chrono::seconds(duration));
  };
}