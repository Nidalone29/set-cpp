cmake_minimum_required(VERSION 3.14)
project(set-cpp LANGUAGES CXX VERSION 0.1)
set(CMAKE_CXX_STANDARD 17)

set(Sources
  ./src/set.cpp
)

set(Headers
  ./src/set.h
)

add_library(${PROJECT_NAME} STATIC ${Sources} ${Headers})

add_subdirectory(test)

