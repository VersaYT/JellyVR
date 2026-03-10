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

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
include(GitVersion)
get_version_from_git()
configure_file(
    ${CMAKE_SOURCE_DIR}/native/src/jellyvr/include/version.h.in
    ${CMAKE_SOURCE_DIR}/native/src/jellyvr/include/version.h
    @ONLY
)

# Godot-cpp binding
set(GODOTCPP_DISABLE_EXCEPTIONS OFF CACHE BOOL "Enable exception handling in godot-cpp" FORCE)
add_subdirectory(${GODOT_CPP_PATH} godot-cpp-build)

# Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    native/src/jellyvr/*.cpp
    native/src/jellyvr/third_party/zip/zip.c
    native/src/jellyvr/*.h
)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    find_package(CURL CONFIG REQUIRED)

    add_library(${PROJECT_NAME} SHARED ${SOURCES})
    target_link_libraries(${PROJECT_NAME}
    PRIVATE
        godot-cpp
        CURL::libcurl
        nlohmann_json::nlohmann_json
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}.${PLATFORM}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/godot_project/bin/
    )

    target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${GODOT_CPP_PATH}/include
        ${CMAKE_CURRENT_BINARY_DIR}
        ${nlohmann_json_SOURCE_DIR}/include/nlohmann
        ${CURL_INCLUDE_DIRS}
    )

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

    set(CURL_INCLUDE_DIR "/usr/include/x86_64-linux-gnu/curl")
    set(CURL_LIBRARY "/lib/x86_64-linux-gnu/libcurl.so")

    add_library(${PROJECT_NAME} SHARED ${SOURCES})
    target_link_libraries(${PROJECT_NAME}
    PRIVATE
        godot-cpp
        CURL::libcurl
        nlohmann_json::nlohmann_json
    )

    # Add curl or other deps here
    find_package(CURL REQUIRED)

    set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}.${PLATFORM}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/godot_project/bin/
    )

    target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${GODOT_CPP_PATH}/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${nlohmann_json_SOURCE_DIR}/include/nlohmann
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")

    #
    # Import libcurl (static)
    #
    add_library(curl STATIC IMPORTED)
    set_target_properties(curl PROPERTIES
        IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/native/src/jellyvr/third_party/aarch64_android/curl/libcurl.a
        INTERFACE_INCLUDE_DIRECTORIES
            ${CMAKE_SOURCE_DIR}/native/src/jellyvr/third_party/aarch64_android/curl/include
    )

    #
    # Import OpenSSL (static)
    #
    add_library(ssl STATIC IMPORTED)
    set_target_properties(ssl PROPERTIES
        IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/native/src/jellyvr/third_party/aarch64_android/openssl/libssl.a
        INTERFACE_INCLUDE_DIRECTORIES
            ${CMAKE_SOURCE_DIR}/native/src/jellyvr/third_party/aarch64_android/openssl/include
    )

    add_library(crypto STATIC IMPORTED)
    set_target_properties(crypto PROPERTIES
        IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/native/src/jellyvr/third_party/aarch64_android/openssl/libcrypto.a
        INTERFACE_INCLUDE_DIRECTORIES
            ${CMAKE_SOURCE_DIR}/native/src/jellyvr/third_party/aarch64_android/openssl/include
    )

    #
    # Import your custom zlib (STATIC)
    #
    add_library(zlib STATIC IMPORTED)
    set_target_properties(zlib PROPERTIES
        IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/native/src/jellyvr/third_party/aarch64_android/zlib/libz.a
        INTERFACE_INCLUDE_DIRECTORIES
            ${CMAKE_SOURCE_DIR}/native/src/jellyvr/third_party/aarch64_android/zlib/include
    )
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
    target_link_libraries(${PROJECT_NAME}
    PRIVATE
        godot-cpp
        curl
        ssl
        crypto
        zlib
        nlohmann_json::nlohmann_json
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}.${PLATFORM}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/godot_project/bin/arm64/
    )

    target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${GODOT_CPP_PATH}/include
        ${CMAKE_CURRENT_BINARY_DIR}
        ${nlohmann_json_SOURCE_DIR}/include/nlohmann
    )
endif()


# Compiler settings
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -fexceptions)

# Platform-specific settings
