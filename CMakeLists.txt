cmake_minimum_required(VERSION 3.15)
project(jellyvr)

# Replace with your Godot 4.x install path
set(GODOT_CPP_PATH "engine/godot-cpp")
set(PROJECT_NAME jellyvr)

# Detect platform
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(PLATFORM android)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(PLATFORM windows)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM linux)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(PLATFORM macos)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Godot-cpp binding
add_subdirectory(${GODOT_CPP_PATH} godot-cpp-build)

# Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    native/src/jellyvr/*.cpp
    native/src/jellyvr/*.h
)

# Output
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} godot-cpp)

# Add curl or other deps here
find_package(CURL REQUIRED)
target_link_libraries(${PROJECT_NAME} CURL::libcurl)

# Compiler settings
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE ${GODOT_CPP_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Platform-specific settings
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}.${PLATFORM}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/godot_project/bin/
)