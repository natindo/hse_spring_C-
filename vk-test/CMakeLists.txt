cmake_minimum_required(VERSION 3.20)
project(GraphShortestPath)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic -O2)

add_executable(graph_program src/main.cpp src/graph.cpp src/graph_io.cpp)

include_directories(./headers/)

find_program(CPPCHECK cppcheck)
if(CPPCHECK)
    add_custom_target(
            cppcheck
            COMMAND ${CPPCHECK} --enable=all --inconclusive --std=c++20 --language=c++ ${CMAKE_SOURCE_DIR}/main.cpp ${CMAKE_SOURCE_DIR}/graph.cpp ${CMAKE_SOURCE_DIR}/graph_io.cpp
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running cppcheck to analyze source code"
            VERBATIM
    )
endif()

#add_subdirectory(test/lib/googletest-main)
enable_testing()
find_package(GTest REQUIRED)

add_executable(runTests test/test_graph.cpp test/test_graph_io.cpp src/graph.cpp src/graph_io.cpp)

target_link_libraries(runTests PRIVATE
        GTest::gtest
        GTest::gtest_main
        pthread
)

add_test(NAME GraphTests COMMAND runTests)
