cmake_minimum_required(VERSION 3.9)

project(eventX LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(eventX SHARED
  src/Event.cpp
  src/EventBus.cpp
  include/Event.h
  include/EventBus.h
  include/EventListener.h
  include/LocalEventListener.h
  include/Lockable.h
)

target_include_directories(eventX PRIVATE .)
target_include_directories(eventX PRIVATE src)
target_include_directories(eventX PUBLIC include)

install(TARGETS eventX DESTINATION lib)
