cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

project(rex_engine
        VERSION 4.3
        DESCRIPTION "Rex Game Engine"
        LANGUAGES C CXX
)

# Build options
option(REX_TOOLS "Build editor tools" ON)
option(REX_DEV_MODE "Enable developer mode" OFF)
set(REX_PLATFORM "linux" CACHE STRING "Target platform: linux, windows, macos")
set(REX_BUILD_TYPE "release" CACHE STRING "Build type: debug, release_debug, release")

# Validate platform
set(VALID_PLATFORMS "linux" "windows" "macos")
list(FIND VALID_PLATFORMS "${REX_PLATFORM}" PLATFORM_INDEX)
if(PLATFORM_INDEX EQUAL -1)
    message(FATAL_ERROR "Invalid platform: ${REX_PLATFORM}. Choose from: ${VALID_PLATFORMS}")
endif()

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Directory setup
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CORE_DIR ${SOURCE_DIR}/core)
set(SERVERS_DIR ${SOURCE_DIR}/servers)
set(EDITOR_DIR ${SOURCE_DIR}/editor)
set(MODULES_DIR ${SOURCE_DIR}/modules)
set(PLATFORM_DIR ${SOURCE_DIR}/platform)
set(SCENE_DIR ${SOURCE_DIR}/scene)
set(THIRDPARTY_DIR ${SOURCE_DIR}/thirdparty)

# Core sources
file(GLOB_RECURSE CORE_SOURCES CONFIGURE_DEPENDS
        ${CORE_DIR}/*.cpp
        ${SERVERS_DIR}/*.cpp
        ${SCENE_DIR}/*.cpp
        ${THIRDPARTY_DIR}/*.c
        ${THIRDPARTY_DIR}/*.cpp
)
file(GLOB_RECURSE CORE_HEADERS CONFIGURE_DEPENDS
        ${CORE_DIR}/*.hpp
        ${SERVERS_DIR}/*.hpp
        ${SCENE_DIR}/*.hpp
)

# Editor sources
if(REX_TOOLS)
    file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS ${EDITOR_DIR}/*.cpp)
    file(GLOB_RECURSE EDITOR_HEADERS CONFIGURE_DEPENDS ${EDITOR_DIR}/*.hpp)
    list(APPEND CORE_SOURCES ${EDITOR_SOURCES})
    list(APPEND CORE_HEADERS ${EDITOR_HEADERS})
endif()

# Platform-specific sources and defines
if(REX_PLATFORM STREQUAL "linux")
    file(GLOB_RECURSE PLATFORM_SOURCES CONFIGURE_DEPENDS ${PLATFORM_DIR}/linuxbsd/*.cpp)
    set(PLATFORM_INCLUDE ${PLATFORM_DIR}/linuxbsd)
    set(PLATFORM_DEFINES UNIX_ENABLED X11_ENABLED OPENGL_ENABLED)
elseif(REX_PLATFORM STREQUAL "windows")
    file(GLOB_RECURSE PLATFORM_SOURCES CONFIGURE_DEPENDS ${PLATFORM_DIR}/windows/*.cpp)
    set(PLATFORM_INCLUDE ${PLATFORM_DIR}/windows)
    set(PLATFORM_DEFINES WIN32_ENABLED NOMINMAX WIN32_LEAN_AND_MEAN OPENGL_ENABLED)
elseif(REX_PLATFORM STREQUAL "macos")
    file(GLOB_RECURSE PLATFORM_SOURCES CONFIGURE_DEPENDS ${PLATFORM_DIR}/macos/*.cpp)
    set(PLATFORM_INCLUDE ${PLATFORM_DIR}/macos)
    set(PLATFORM_DEFINES MACOS_ENABLED OPENGL_ENABLED)
endif()

list(APPEND CORE_SOURCES ${PLATFORM_SOURCES})

# Modules
file(GLOB_RECURSE MODULE_SOURCES CONFIGURE_DEPENDS
        ${MODULES_DIR}/gdscript/*.cpp
        ${MODULES_DIR}/gridmap/*.cpp
        ${MODULES_DIR}/navigation/*.cpp
        ${MODULES_DIR}/visual_script/*.cpp
)
list(APPEND CORE_SOURCES ${MODULE_SOURCES})

# Third-party C sources
file(GLOB_RECURSE THIRDPARTY_SOURCES CONFIGURE_DEPENDS
        ${THIRDPARTY_DIR}/zlib/*.c
        ${THIRDPARTY_DIR}/libpng/*.c
        ${THIRDPARTY_DIR}/freetype/*.c
        ${THIRDPARTY_DIR}/libogg/*.c
        ${THIRDPARTY_DIR}/libvorbis/*.c
)
list(APPEND CORE_SOURCES ${THIRDPARTY_SOURCES})

# Define executable
add_executable(rex_engine ${CORE_SOURCES} ${CORE_HEADERS})

# Output name
set(OUTPUT_NAME "rex.${REX_PLATFORM}")
if(REX_TOOLS)
    set(OUTPUT_NAME "${OUTPUT_NAME}.editor")
else()
    set(OUTPUT_NAME "${OUTPUT_NAME}.opt")
endif()
if(REX_BUILD_TYPE MATCHES "debug")
    set(OUTPUT_NAME "${OUTPUT_NAME}.debug")
endif()
set_target_properties(rex_engine PROPERTIES OUTPUT_NAME "${OUTPUT_NAME}")

# Compiler flags
if(MSVC)
    if(REX_BUILD_TYPE MATCHES "debug")
        target_compile_options(rex_engine PRIVATE /Od /Zi)
    else()
        target_compile_options(rex_engine PRIVATE /O2 /DNDEBUG)
    endif()
else()
    if(REX_BUILD_TYPE MATCHES "debug")
        target_compile_options(rex_engine PRIVATE -g -O0)
    else()
        target_compile_options(rex_engine PRIVATE -O3 -DNDEBUG)
    endif()
endif()

# Include directories
target_include_directories(rex_engine PRIVATE
        ${SOURCE_DIR}
        ${CORE_DIR}
        ${CORE_DIR}/binder
        ${CORE_DIR}/io
        ${CORE_DIR}/math
        ${CORE_DIR}/os
        ${CORE_DIR}/config
        ${SERVERS_DIR}
        ${SERVERS_DIR}/rendering
        ${SERVERS_DIR}/audio
        ${SERVERS_DIR}/physics
        ${EDITOR_DIR}
        ${MODULES_DIR}
        ${MODULES_DIR}/gdscript
        ${MODULES_DIR}/gridmap
        ${MODULES_DIR}/navigation
        ${MODULES_DIR}/visual_script
        ${PLATFORM_DIR}
        ${PLATFORM_INCLUDE}
        ${SCENE_DIR}
        ${SCENE_DIR}/2d
        ${SCENE_DIR}/3d
        ${SCENE_DIR}/animation
        ${THIRDPARTY_DIR}
        ${THIRDPARTY_DIR}/zlib
        ${THIRDPARTY_DIR}/libpng
        ${THIRDPARTY_DIR}/freetype
        ${THIRDPARTY_DIR}/libogg
        ${THIRDPARTY_DIR}/libvorbis
)

# Platform-specific dependencies
if(REX_PLATFORM STREQUAL "linux")
    find_package(X11 REQUIRED)
    find_package(ALSA REQUIRED)
    find_package(Freetype REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
    pkg_check_modules(LIBPULSE REQUIRED libpulse)

    target_link_libraries(rex_engine PRIVATE
            ${X11_LIBRARIES}
            ${ALSA_LIBRARIES}
            ${FREETYPE_LIBRARIES}
            ${FONTCONFIG_LIBRARIES}
            ${LIBPULSE_LIBRARIES}
            pthread
            dl
            z
    )
elseif(REX_PLATFORM STREQUAL "windows")
    target_link_libraries(rex_engine PRIVATE
            ws2_32
            winmm
            dsound
            opengl32
            d3d9
            shlwapi
            dwmapi
            gdi32
    )
elseif(REX_PLATFORM STREQUAL "macos")
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    find_library(AUDIOUNIT_LIBRARY AudioUnit)
    target_link_libraries(rex_engine PRIVATE
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${OPENGL_LIBRARY}
            ${COREFOUNDATION_LIBRARY}
            ${AUDIOUNIT_LIBRARY}
            z
    )
endif()

# Preprocessor definitions
target_compile_definitions(rex_engine PRIVATE ${PLATFORM_DEFINES})

# Dev mode defines
if(REX_DEV_MODE)
    target_compile_definitions(rex_engine PRIVATE DEV_ENABLED TOOLS_ENABLED)
endif()

# Output binary directory
set_target_properties(rex_engine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)
