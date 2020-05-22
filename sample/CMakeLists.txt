cmake_minimum_required(VERSION 3.5)

project(sample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/../eventX/include)
link_directories(${CMAKE_SOURCE_DIR}/../eventX/lib)

add_executable(sample
    main.cpp
    Foobar.h
)

target_link_libraries(sample eventX)
