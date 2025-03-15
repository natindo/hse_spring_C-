cmake_minimum_required(VERSION 3.20)
project(sem1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(test/lib/googletest-main)

add_library(sem1_lib
        src/matrix.cpp
)

target_include_directories(sem1_lib PUBLIC
        ./headers
)

add_executable(sem1 src/main.cpp)

target_link_libraries(sem1 PRIVATE sem1_lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Werror -Wall -Wextra -Wpedantic")


set(VALID_BUILD_TYPES Debug Coverage)

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${VALID_BUILD_TYPES})

if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

#find_package(GTest REQUIRED)
enable_testing()

add_executable(test_sem1
        test/sem1_test.cpp
)

target_link_libraries(test_sem1 PRIVATE
        sem1_lib
        GTest::gtest
        GTest::gtest_main
        pthread
)

add_test(NAME test_sem1 COMMAND test_sem1)

