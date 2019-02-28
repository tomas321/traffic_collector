cmake_minimum_required(VERSION 3.13)
project(traffic_collector)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DBOOST_LOG_DYN_LINK)

include_directories(src ${Boost_INCLUDE_DIR})

add_executable(traffic_collector src/logging.cpp src/logging.h src/input/controller.cpp src/input/controller.h)

find_package(Boost 1.65 COMPONENTS log thread system REQUIRED)
find_package(Threads)
target_link_libraries(traffic_collector ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# target_link_libraries(${Boost_LOG_LIBRARY} ${Boost_THREAD_LIBRARY})

include_directories(${Boost_INCLUDE_DIR})