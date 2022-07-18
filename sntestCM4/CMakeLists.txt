cmake_minimum_required(VERSION 3.14)
project(test LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

find_package(fmt REQUIRED)
find_package(Threads REQUIRED)

include_directories(
    ${fmt_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/share/zos/project/include
    ${CMAKE_CURRENT_SOURCE_DIR}/share
    ${CMAKE_CURRENT_SOURCE_DIR}/share/utils
)

set(LIBS
    ${LIBS}
    Threads::Threads
    fmt::fmt
    wiringPi
)

file(GLOB TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)
file(GLOB SHARED_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/share/udp/*.cpp
)

# foreach(test_file ${TEST_FILES})
#     file(RELATIVE_PATH test_file_name ${CMAKE_CURRENT_SOURCE_DIR} ${test_file})
#     string(REPLACE ".cpp" "" test_name ${test_file_name})
#     # string(REPLACE "test/" "" test_name ${test_name})
#     message("test file: " ${test_file} " -- " ${test_name})

#     add_executable(${test_name}
#         ${test_file_name}
#         ${SHARED_SOURCE_FILES}
#     )
#     target_link_libraries(${test_name} ${LIBS})
# endforeach()

add_executable(main ${TEST_FILES})
target_link_libraries(main ${LIBS})