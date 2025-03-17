cmake_minimum_required(VERSION 3.16)
project(JPO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Conan dependencies
include(${CMAKE_BINARY_DIR}/generators/conan_toolchain.cmake)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(include)

add_executable(JPO 
    src/main.cpp 
    src/mainwindow.cpp
)

target_link_libraries(JPO Qt6::Widgets)
