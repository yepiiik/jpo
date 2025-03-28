cmake_minimum_required(VERSION 3.15)
project(jpo-app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
# set(VCPKG_TARGET_TRIPLET x64-windows-static)

# Find required packages
find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

# Get all source files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

# Add executable
add_executable(jpo-app ${SOURCES})

# Include directories
target_include_directories(jpo-app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(jpo-app PRIVATE
    Qt5::Widgets
    Qt5::Network
) 