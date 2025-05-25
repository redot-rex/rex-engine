cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

project(rex_engine
        VERSION 4.3
        DESCRIPTION "Rex Game Engine"
        LANGUAGES C CXX
)

find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(PYTHON_EXECUTABLE "python3" CACHE STRING "python3")
# Build options
option(REX_TOOLS "Build editor tools" OFF)
option(REX_DEV_MODE "Enable developer mode" OFF)
set(REX_PLATFORM "linux" CACHE STRING "Target platform: linux, windows, macos")
set(REX_BUILD_TYPE "release" CACHE STRING "Build type: debug, release_debug, release")

option(PRODUCTION "Set defaults to build Redot for use in production" OFF)
option(THREADS "Enable Threading Support" ON)

# Components

option(DEPRICATED "Enable compatibility code of deprecated and removed features" ON)
set(PRECISION "single" CACHE STRING "Set the floating-point precision level: single, double")
option(MINIZIP "Enable ZIP archive support using minizip" ON)
option(BROTLI "Enable Brotli for decompression and WOFF2 fonts support" ON)
option(XAUDIO2 "Enable XAudio2 audio driver on supported platforms" OFF)
option(VULKAN "Enable the vulkan rendering driver" ON)
option(OPENGL3 "Enable the OpenGL/GLES3 rendering driver" ON)
option(D3D12 "Enable the Direct3D 12 rendering driver on supported platforms" OFF)
option(METAL "Enable the Metal rendering driver on supported platforms (Apple arm64 only)" OFF)
option(USE_VOLK "Use the volk library to load the Vulkan loader dynamically" ON)
option(DISABLE_EXCEPTIONS "Force disabling exception handling code" ON)

# Advanced options
option(REX_TESTS "Build the unit tests" OFF)

# Third Party options

option(BUILTIN_BROTLI "Use the built-in Brotli library" ON)
option(BUILTIN_CERTS "Use the built-in SSL certificates bundles" ON)
option(BUILTIN_CLIPPER2 "Use the built-in Clipper2 library" ON)
option(BUILTIN_EMBREE "Use the built-in Embree library" ON)
option(BUILTIN_ENET "Use the built-in ENet library" ON)
option(BUILTIN_FREETYPE "Use the built-in FreeType library" ON)
option(BUILTIN_MSDFGEN "Use the built-in MSDFgen library" ON)
option(BUILTIN_GLSLANG "Use the built-in glslang library" ON)
option(BUILTIN_GRAPHITE "Use the built-in Graphite library" ON)
option(BUILTIN_HARFBUZZ "Use the built-in HarfBuzz library" ON)
option(BUILTIN_ICU4C "Use the built-in ICU library" ON)
option(BUILTIN_LIBOGG "Use the built-in libogg library" ON)
option(BUILTIN_LIBPNG "Use the built-in libpng library" ON)
option(BUILTIN_LIBTHEORA "Use the built-in libtheora library" ON)
option(BUILTIN_LIBVORBIS "Use the built-in libvorbis library" ON)
option(BUILTIN_LIBWEBP "Use the built-in libwebp library" ON)
option(BUILTIN_WSLAY "Use the built-in wslay library" ON)
option(BUILTIN_MBEDTLS "Use the built-in mbedTLS library" ON)
option(BUILTIN_MINIUPNPC "Use the built-in miniupnpc library" ON)
option(BUILTIN_OPENXR "Use the built-in OpenXR library" ON)
option(BUILTIN_PCRE2 "Use the built-in PCRE2 library" ON)
option(BUILTIN_PCRE2_WITH_JIT "Use JIT compiler for the built-in PCRE2 library" ON)
option(BUILTIN_RECASTNAVIGATION "Use the built-in Recast navigation library" ON)
option(BUILTIN_RVO2_2D "Use the built-in RVO2 2D library" ON)
option(BUILTIN_RVO2_3D "Use the built-in RVO2 3D library" ON)
option(BUILTIN_XATLAS "Use the built-in xatlas library" ON)
option(BUILTIN_ZLIB "Use the built-in zlib library" ON)
option(BUILTIN_ZSTD "Use the built-in Zstd library" ON)

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

# Define executable
add_executable(rex_engine main/main.cpp)
target_include_directories(rex_engine PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

if (REX_TOOLS)
	target_compile_definitions(rex_engine PUBLIC
		TOOLS_ENABLED
		ENGINE_UPDATE_CHECK_ENABLED
	)
endif()

if (REX_DEV_MODE)
	target_compile_definitions(rex_engine PUBLIC
		DEV_ENABLED
		DEBUG_ENABLED
	)
else()
	target_compile_definitions(rex_engine PUBLIC
		NODEBUG
	)
endif()

if (NOT DEPRICATED)
	target_compile_definitions(rex_engine PUBLIC
		DISABLE_DEPRECATED
	)
endif()

if (PRECISION STREQUAL "double")
	target_compile_definitions(rex_engine PUBLIC
		REAL_T_IS_DOUBLE
	)
endif()

add_subdirectory(thirdparty)
add_subdirectory(core)
add_subdirectory(servers)
add_subdirectory(scene)
if (REX_TOOLS)
	add_subdirectory(editor)
endif()
add_subdirectory(drivers)
add_subdirectory(platform)
add_subdirectory(modules)
add_subdirectory(main)

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

# Preprocessor definitions
target_compile_definitions(rex_engine PRIVATE ${PLATFORM_DEFINES})

# Output binary directory
set_target_properties(rex_engine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)
