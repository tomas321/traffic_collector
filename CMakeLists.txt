cmake_minimum_required(VERSION 3.13)
project(traffic_collector)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DBOOST_LOG_DYN_LINK)

add_executable(main bin/main.cpp
        src/logging.cpp
        include/logging.h
        src/input/controller.cpp
        include/controller.h
        src/configuration.cpp
        include/configuration.h
        src/output/databese_controller.cpp
        include/databese_controller.h include/exceptions.h)

find_package(Boost 1.65 COMPONENTS log thread system REQUIRED)
find_package(Threads REQUIRED)
find_package(yaml-cpp 0.5 REQUIRED)

target_link_libraries(main ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} yaml-cpp)
include_directories(${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIR} ${YAML_CPP_INCLUDE_DIR})
