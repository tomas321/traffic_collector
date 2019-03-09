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
        include/databese_controller.h
        include/exceptions.h
        include/validate.h
        src/validation/validate.cpp)

find_package(Boost 1.65 COMPONENTS log log_setup thread system filesystem REQUIRED)
find_package(Threads REQUIRED)
find_package(yaml-cpp 0.5 REQUIRED)

target_link_libraries(main ${Boost_LOG_LIBRARY}
        ${Boost_LOG_SETUP_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        yaml-cpp)
# target_link_libraries(${Boost_LIBRARIES})
include_directories(${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDE_DIR} ${YAML_CPP_INCLUDE_DIR})

add_executable(validate bin/validation.cpp
        src/validation/validate.cpp
        include/validate.h
        include/exceptions.h)
target_link_libraries(validate
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        yaml-cpp)