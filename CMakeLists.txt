cmake_minimum_required(VERSION 3.0)
project(traffic_collector)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(yaml-cpp 0.5 REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include
        ${YAML_CPP_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/lib)

file(GLOB_RECURSE sources src/*.cpp include/*.h)
file(GLOB_RECURSE parsing_lib src/process/parsing/*.cpp include/parsing/*.h)
set(libgeoip GeoIP)
set(common_libs
        ${CMAKE_THREAD_LIBS_INIT}
        yaml-cpp
        pcap
        ${libgeoip})

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
        src/output/databese_controller.cpp
        src/logging.cpp)

## GEOIP ##
add_executable(geoip bin/geoip.cpp
        include/harmonization.h
        src/process/harmonization.cpp)
target_link_libraries(geoip ${libgeoip})

## TESTS ##
add_executable(test_packet_parsing test/test_example.cpp test/test_main.cpp src/logging.cpp include/logging.h ${parsing_lib})
add_executable(test_json_lib test/test_json_lib.cpp test/test_main.cpp src/process/json.cpp include/json.h)
add_executable(test_log test/test_logging.cpp test/test_main.cpp src/logging.cpp include/logging.h)
add_executable(test_geoip test/geoip_test.cpp test/test_main.cpp include/harmonization.h src/process/harmonization.cpp)
target_link_libraries(test_geoip ${libgeoip})
add_executable(test_operations test/test_operations.cpp test/test_main.cpp include/parsing/operations.h)

