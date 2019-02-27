cmake_minimum_required(VERSION 3.13)
project(traffic_collector)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(traffic_collector src/logging.cpp src/logging.h src/input/controller.cpp src/input/controller.h)