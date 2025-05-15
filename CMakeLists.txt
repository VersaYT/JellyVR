cmake_minimum_required(VERSION 3.15)
project(jellyvr)
include(FetchContent)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)

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
set(GODOTCPP_DISABLE_EXCEPTIONS OFF CACHE BOOL "Enable exception handling in godot-cpp" FORCE)
add_subdirectory(${GODOT_CPP_PATH} godot-cpp-build)

# Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    native/src/jellyvr/*.cpp
    native/src/jellyvr/*.h
)

# Output
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        godot-cpp
        CURL::libcurl
        nlohmann_json::nlohmann_json
)

# Add curl or other deps here
find_package(CURL REQUIRED)

# Compiler settings
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -fexceptions)
target_include_directories(${PROJECT_NAME} PRIVATE ${GODOT_CPP_PATH}/include)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${nlohmann_json_SOURCE_DIR}/include/nlohmann
)

# Platform-specific settings
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}.${PLATFORM}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/godot_project/bin/
)