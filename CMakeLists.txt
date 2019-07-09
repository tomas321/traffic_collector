cmake_minimum_required(VERSION 3.0)
project(traffic_collector)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(yaml-cpp 0.5 REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/include
        ${YAML_CPP_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/lib)
link_directories(/usr/local/lib)

file(GLOB_RECURSE sources src/*.cpp include/*.h)
file(GLOB_RECURSE parsing_lib src/process/parsing/*.cpp include/parsing/*.h)
set(libgeoip GeoIP)
set(common_libs
        ${CMAKE_THREAD_LIBS_INIT}
        yaml-cpp
        pcap
        ${libgeoip})

## MAIN ##
add_executable(traffcol bin/traffcol.cpp ${sources})
target_link_libraries(traffcol ${common_libs})

## CONFIG ##
add_executable(traffcol-print-config bin/traffcol-print-config.cpp ${sources})
target_link_libraries(traffcol-print-config ${common_libs})

## VALIDATE ##
add_executable(traffcol-validate bin/traffcol-validate.cpp ${sources})
target_link_libraries(traffcol-validate ${common_libs})
