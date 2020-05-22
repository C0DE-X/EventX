cmake_minimum_required(VERSION 3.9)

project(eventX LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(eventX SHARED
  src/EventX.cpp
  include/EventX.h
  include/EventX_globals.h
  include/Event.h
  include/EventListener.h
)

target_include_directories(eventX PRIVATE .)
target_include_directories(eventX PRIVATE src)
target_include_directories(eventX PUBLIC include)

install(TARGETS eventX DESTINATION lib)
