cmake_minimum_required(VERSION 3.13)
project(traffic_collector)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DBOOST_LOG_DYN_LINK)

find_package(Boost 1.65 COMPONENTS log log_setup thread system REQUIRED)
find_package(Threads REQUIRED)
find_package(yaml-cpp 0.5 REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIR}
        ${YAML_CPP_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/lib)

file(GLOB_RECURSE sources src/*.cpp include/*.h)
file(GLOB_RECURSE parsing_lib src/process/parsing/*.cpp include/parsing/*.h)
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

## CONFIG ##
add_executable(load_config bin/load_config.cpp ${sources})
target_link_libraries(load_config ${common_libs})

## VALIDATE ##
add_executable(validate bin/validation.cpp ${sources})
target_link_libraries(validate ${common_libs})

## COLLECT ##
add_executable(collect_net bin/collect.cpp ${sources})
target_link_libraries(collect_net ${common_libs})

## JSON ##
add_executable(json bin/json.cpp
        include/json.h
        src/process/json.cpp)

## Sender ##
add_executable(sender bin/sender.cpp
        include/databese_controller.h
        src/output/databese_controller.cpp)

## TESTS ##
add_executable(test_acpket_parsing test/test_example.cpp test/test_main.cpp ${parsing_lib})
add_executable(test_json_lib test/test_json_lib.cpp test/test_main.cpp src/process/json.cpp include/json.h)
