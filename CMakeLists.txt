cmake_minimum_required(VERSION 3.10)
project(Pbscpp_task6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Werror")

add_executable(main main.cpp)
target_link_libraries(main gtest pthread)
