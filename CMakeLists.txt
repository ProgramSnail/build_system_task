cmake_minimum_required(VERSION 3.10)

project(BuildSystem)


set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


set(BUILD_SYSTEM_SOURCES 
		build_graph.cpp 
		builder.cpp 
		thread_pool.cpp
		task_queue.cpp
		dependency_manager.cpp
		task.hpp)

set (CLI_SOURCES
		main.cpp
		interactive_mode.cpp
		file_mode.cpp
		cli_common.cpp
		test_tasks.hpp)

add_library(BuildSystemLib ${BUILD_SYSTEM_SOURCES})

add_executable(BuildSystemCli ${CLI_SOURCES})
target_link_libraries(BuildSystemCli BuildSystemLib)

set_property(TARGET BuildSystemLib PROPERTY CXX_STANDARD 20)
set_property(TARGET BuildSystemCli PROPERTY CXX_STANDARD 20)
