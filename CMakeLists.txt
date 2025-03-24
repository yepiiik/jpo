cmake_minimum_required(VERSION 3.15)
project(jpo-app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required packages
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# Add executable
add_executable(jpo-app
    src/main.cpp
    src/weather_service.cpp
    src/mainwindow.cpp
    src/mainwindow.h
)

# Include directories
target_include_directories(jpo-app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(jpo-app PRIVATE
    Qt5::Widgets
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

# Copy resources to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR}) 