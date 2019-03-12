cmake_minimum_required(VERSION 3.13)
project(traffic_collector)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DBOOST_LOG_DYN_LINK)

find_package(Boost 1.65 COMPONENTS log log_setup thread system REQUIRED)
find_package(Threads REQUIRED)
find_package(yaml-cpp 0.5 REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIR} ${YAML_CPP_INCLUDE_DIR})

file(GLOB_RECURSE sources src/*.cpp include/*.h)
set(common_libs
        ${Boost_LOG_LIBRARY}
        ${Boost_LOG_SETUP_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        yaml-cpp
        pcap)

## MAIN ##
add_executable(main bin/main.cpp ${sources})
target_link_libraries(main ${common_libs})

## VALIDATE ##
add_executable(validate bin/validation.cpp ${sources})
target_link_libraries(validate ${common_libs})

## COLLECT ##
add_executable(collect_net bin/collect.cpp ${sources})
target_link_libraries(collect_net ${common_libs})