cmake_minimum_required(VERSION 3.20)
project(HW1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(test/lib/googletest-main)

add_library(HW1_lib
        src/db/db.cpp
        src/app/app.cpp
        src/handlers/response.cpp
        src/handlers/request.cpp
        src/handlers/parser/parser.cpp
)

target_include_directories(HW1_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/headers/app
        ${CMAKE_CURRENT_SOURCE_DIR}/headers/db
        ${CMAKE_CURRENT_SOURCE_DIR}/headers/handlers
        ${CMAKE_CURRENT_SOURCE_DIR}/headers/handlers/parser
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fsanitize=address -fsanitize=leak -Werror -Wall -Wextra -Wpedantic")

add_executable(HW1
        src/main.cpp
)

target_link_libraries(HW1 PRIVATE HW1_lib)


set(VALID_BUILD_TYPES Debug Coverage)

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${VALID_BUILD_TYPES})

if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

find_package(GTest REQUIRED)
enable_testing()

add_executable(test_database
        test/database_test.cpp
)

target_link_libraries(test_database PRIVATE
        HW1_lib
        GTest::gtest
        GTest::gtest_main
        pthread
)

add_test(NAME test_database COMMAND test_database)
