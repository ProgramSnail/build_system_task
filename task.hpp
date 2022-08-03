#pragma once

#include <functional>
#include <utility>

namespace build_system {

using Task = std::function<void()>;

using IdentTask = std::pair<Task, size_t>;

}; // namespace build_system