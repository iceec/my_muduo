cmake_minimum_required(VERSION 3.10)



project(net)

set(CXX_STANDARD 17)

set(CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(./src)

add_executable(net  ./main.cpp)

target_link_libraries(net PRIVATE mymuduo)


