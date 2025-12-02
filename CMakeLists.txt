cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

if (DEFINED ENV{GITHUB_ACTIONS})
    set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)
endif()

project(Named-Editor-Groups VERSION 1.0.0)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

file(GLOB_RECURSE SRC_MAIN
    "${SRC_DIR}/**.cpp"
)

add_library(${PROJECT_NAME} SHARED "${SRC_MAIN}")
# allow easy inclusion of stuff
target_include_directories(${PROJECT_NAME}
    PRIVATE ${SRC_DIR}/base64
    PRIVATE ${SRC_DIR}/data
    PRIVATE ${SRC_DIR}/parser
    PRIVATE ${SRC_DIR}/types
    PRIVATE ${SRC_DIR}/utils
)

target_include_directories(${PROJECT_NAME} PUBLIC api/)

target_precompile_headers(${PROJECT_NAME} PUBLIC "${SRC_DIR}/prelude.hpp")

if (PROJECT_IS_TOP_LEVEL)
    target_compile_definitions(${PROJECT_NAME} PRIVATE SPAGHETTDEV_NAMED_EDITOR_GROUPS_EXPORTING)
endif()

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})

# Debug macro
if (NOT DEFINED ENV{GITHUB_ACTIONS})
    target_compile_definitions(
        ${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:NID_DEBUG_BUILD>
            $<$<CONFIG:RelWithDebInfo>:NID_DEBUG_BUILD>
    )
endif()
