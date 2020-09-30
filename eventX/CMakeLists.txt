cmake_minimum_required(VERSION 3.9)

project(eventX LANGUAGES CXX)

add_library(eventX SHARED
  src/Event.cpp
  src/EventBus.cpp
  src/Timer.cpp
  include/Event.h
  include/EventBus.h
  include/EventListener.h
  include/LocalEventListener.h
  include/Lockable.h
  include/Timer.h
)

set_target_properties(eventX PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    ARCHIVE_OUTPUT_DIRECTORY "bin/"
    RUNTIME_OUTPUT_DIRECTORY "bin/"
    LIBRARY_OUTPUT_DIRECTORY "lib/"
    )

target_include_directories(eventX PRIVATE . src)
target_include_directories(eventX PUBLIC include)

install(TARGETS eventX DESTINATION lib)
