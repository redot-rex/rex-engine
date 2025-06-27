cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(methods.cmake)
include(version.cmake)

# TODO: Only checking for desktop platforms for now.
# NOTE: Using strings that make the Scons build system.
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux|DragonFly|FreeBSD|NetBSD|OpenBSD")
	set(REX_PLATFORM "linuxbsd")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	set(REX_PLATFORM "windows")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
	set(REX_PLATFORM "macos")
else()
	message(FATAL_ERROR "${CMAKE_HOST_SYSTEM_NAME} not supported.")
endif()

message(STATUS "Detected build platform: ${REX_PLATFORM}")
message(STATUS "Architecture: ${CMAKE_GENERATOR_PLATFORM}")

# TODO: Need a way to detect the architecture.

set(ARCH "x86_64")

# Build options

set(OPTIMIZE "auto" CACHE STRING "Optimization level (by default inferred from 'target' and 'dev_build'): auto, none,
custom, debug, speed, speed_trace, size, size_extra")
set(BUILD_TARGET "editor" CACHE STRING "Compilation target: editor, template_release, template_debug")

option(REX_TOOLS "Build editor tools" OFF)
option(REX_DEV_BUILD "Enable developer build" OFF)

option(DEBUG_SYMBOLS "Build with debugging symbols" OFF)
option(SEPARATE_DEBUG_SYMBOLS "Extract debugging symbols to a separate file" OFF)
option(DEBUG_RELATIVE_PATHS "Make file paths in debug symbols relative (if supported)" OFF)

set(LTO "none" CACHE STRING "Link-time optimization (production builds): none, auto, thin, full")

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

option(DISABLE_3D "Disable 3D nodes for a smaller executable" OFF)
option(DISABLE_ADVANCED_GUI "Disable advanced GUI nodes and behaviors" OFF)
option(DISABLE_PHYSICS_2D "Disable 2D physics nodes and server" OFF)
option(DISABLE_PHYSICS_3D "Disable 3D physics nodes and server" OFF)
option(DISABLE_NAVIGATION_2D "Disable 2D navigation features" OFF)
option(DISABLE_NAVIGATION_3D "Disable 3D navigation features" OFF)
option(DISABLE_XR "Disable XR nodes and server" OFF)
option(MODULES_ENABLED_BY_DEFAULT "If no disable all modules except ones explicitly enabled" ON)
option(NO_EDITOR_SPLASH "Don't use the custom splash screen for the editor" ON)
option(USE_PRECISE_MATH_CHECKS "Math checks use very precise epsilon (debug option)" OFF)
option(STRICT_CHECKS "Enforce stricter checks (debug option)" OFF)
option(SCU_BUILD "Use single compilation unit build" OFF)
option(ENGINE_UPDATE_CHECK "Enable engine update checks in the Project Manager" ON)
option(STEAMAPI "Enable minimal SteamAPI integration for usage time tracking (editor only)" OFF)

if (DEBUG_SYMBOLS)
	set(CMAKE_BUILD_TYPE Debug)
else()
	set(CMAKE_BUILD_TYPE Release)
endif()

project(rex_engine
        VERSION 4.3
        DESCRIPTION "Rex Game Engine"
        LANGUAGES C CXX
)

# Executable is defined per platform

if (REX_PLATFORM STREQUAL "linuxbsd")
	add_subdirectory(platform/linuxbsd)
	#elseif(REX_PLATFORM STREQUAL "macos")
	#add_subdirectory(platform/macos)
	#elseif(REX_PLATFORM STREQUAL "windows")
	#add_subdirectory(platform/windows)
endif()

#add_executable(rex_engine main/main.cpp)
target_include_directories(rex_engine PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

if (BUILD_TARGET STREQUAL "editor")
	set(EDITOR_BUILD ON)
endif()

if (BUILD_TARGET STREQUAL "editor" OR BUILD_TARGET STREQUAL "template_debug")
	set(DEBUG_FEATURES ON)
endif()

if (EDITOR_BUILD)
	target_compile_definitions(rex_engine PUBLIC
		TOOLS_ENABLED
	)
	if (ENGINE_UPDATE_CHECK)
		target_compile_definitions(rex_engine PUBLIC
			ENGINE_UPDATE_CHECK_ENABLED
		)
	endif()
	if (NOT EXISTS ${CMAKE_SOURCE_DIR}/main/splash_editor.png)
		set(NO_EDITOR_SPLASH ON)
	endif()
	if (NO_EDITOR_SPLASH)
		target_compile_definitions(rex_engine PUBLIC
			NO_EDITOR_SPLASH
		)
	endif()
endif()

if (DEBUG_FEATURES)
	target_compile_definitions(rex_engine PUBLIC
		DEBUG_ENABLED
	)
endif()

if (REX_DEV_BUILD)
	target_compile_definitions(rex_engine PUBLIC
		DEV_ENABLED
	)
else()
	target_compile_definitions(rex_engine PUBLIC
		NODEBUG
	)
endif()

if (USE_PRECISE_MATH_CHECKS)
	target_compile_definitions(rex_engine PUBLIC
		PRECISE_MATH_CHECKS
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

if (STRICT_CHECKS)
	target_compile_definitions(rex_engine PUBLIC
		STRICT_CHECKS
	)
endif()


# TODO: Missing SCU Build here


if (EDITOR_BUILD)
	if (DISABLE_3D
	 OR DISABLE_ADVANCED_GUI
	 OR DISABLE_PHYSICS_2D
	 OR DISABLE_PHYSICS_3D
	 OR DISABLE_NAVIGATION_2D
	 OR DISABLE_NAVIGATION_3D)
		message(FATAL_ERROR "The following build options cannot be used for editor builds, DISABLE_3D, DISABLE_ADVANCED_GUI,
 DISABLE_PHYSICS_2D, DISABLE_PHYSICS_3D, DISABLE_NAVIGATION_2D, DISABLE_NAVIGATION_3D")
	endif()
endif()

if (DISABLE_3D)
	target_compile_definitions(rex_engine PUBLIC
		_3D_DISABLED
	)
	set(DISABLE_NAVIGATION_3D ON)
	set(DISABLE_PHYSICS_3D ON)
	set(DISABLE_XR ON)
endif()

if (DISABLE_ADVANCED_GUI)
	target_compile_definitions(rex_engine PUBLIC
		ADVANCED_GUI_DISABLED
	)
endif()

if (DISABLE_PHYSICS_2D)
	target_compile_definitions(rex_engine PUBLIC
		PHYSICS_2D_DISABLED
	)
endif()

if (DISABLE_PHYSICS_3D)
	target_compile_definitions(rex_engine PUBLIC
		PHYSICS_3D_DISABLED
	)
endif()

if (DISABLE_NAVIGATION_2D)
	target_compile_definitions(rex_engine PUBLIC
		NAVIGATION_2D_DISABLED
	)
endif()

if (DISABLE_NAVIGATION_3D)
	target_compile_definitions(rex_engine PUBLIC
		NAVIGATION_3D_DISABLED
	)
endif()

if (DISABLE_XR)
	target_compile_definitions(rex_engine PUBLIC
		XR_DISABLED
	)
endif()

if (MINIZIP)
	target_compile_definitions(rex_engine PUBLIC
		MINIZIP_ENABLED
	)
endif()

if (BROTLI)
	target_compile_definitions(rex_engine PUBLIC
		BROTLI_ENABLED
	)
endif()

if (THREADS)
	target_compile_definitions(rex_engine PUBLIC
		THREADS_ENABLED
	)
endif()

if (DISABLE_EXCEPTIONS)
	if (MSVC)
		target_compile_definitions(rex_engine PUBLIC
			_HAS_EXCEPTIONS=0
		)
	else()
		target_compile_options(rex_engine PUBLIC
			-fno-exceptions
		)
	endif()
elseif(MSVC)
	target_compile_options(rex_engine PUBLIC
		/EHsc
	)
endif()

if (REX_PLATFORM STREQUAL "macos")
	target_include_directories(rex_engine PUBLIC
		${CMAKE_SOURCE_DIR}/platform/linuxbsd/
	)
	target_compile_definitions(rex_engine PUBLIC
		MACOS_ENABLED
		UNIX_ENABLED
		COREAUDIO_ENABLED
		COREMIDI_ENABLED
	)
	if (METAL)
		target_compile_definitions(rex_engine PUBLIC
			METAL_ENABLED
			RD_ENABLED
		)
		target_include_directories(rex_engine PUBLIC
			${CMAKE_SOURCE_DIR}/thirdparty/spirv-cross/
		)
	endif()
	if (VULKAN)
		target_compile_definitions(rex_engine PUBLIC
			VULKAN_ENABLED
			RD_ENABLED
		)
	endif()
	if (OPENGL3)
		target_compile_definitions(rex_engine PUBLIC
			GLES3_ENABLED
		)
	endif()
endif()
if (REX_PLATFORM STREQUAL "ios")
	target_include_directories(rex_engine PUBLIC
		${CMAKE_SOURCE_DIR}/platform/ios/
	)
	target_compile_definitions(rex_engine PUBLIC
		IOS_ENABLED
		UNIX_ENABLED
		COREAUDIO_ENABLED
	)
	if (METAL)
		target_compile_definitions(rex_engine PUBLIC
			METAL_ENABLED
			RD_ENABLED
		)
		target_include_directories(rex_engine PUBLIC
			${CMAKE_SOURCE_DIR}/thirdparty/spirv-cross/
		)
	endif()
	if (VULKAN)
		target_compile_definitions(rex_engine PUBLIC
			VULKAN_ENABLED
			RD_ENABLED
		)
	endif()
	if (OPENGL3)
		target_compile_definitions(rex_engine PUBLIC
			GLES3_ENABLED
			GLES_SILENCE_DEPRECATION
		)

	endif()
endif()
if (REX_PLATFORM STREQUAL "windows")
	target_include_directories(rex_engine PUBLIC
		${CMAKE_SOURCE_DIR}/platform/windows/
	)
	target_compile_definitions(rex_engine PUBLIC
		IOS_ENABLED
		UNIX_ENABLED
		COREAUDIO_ENABLED
	)
	if (VULKAN)
		target_compile_definitions(rex_engine PUBLIC
			VULKAN_ENABLED
			RD_ENABLED
		)
	endif()
	if (OPENGL3)
		target_compile_definitions(rex_engine PUBLIC
			GLES3_ENABLED
		)
	endif()
	if (D3D12)
		target_compile_definitions(rex_engine PUBLIC
			D3D12_ENABLED
			RD_ENABLED
		)
	endif()
endif()
# Output name
set(OUTPUT_NAME "rex.${REX_PLATFORM}")
if(EDITOR_BUILD)
    set(OUTPUT_NAME "${OUTPUT_NAME}.editor")
else()
    set(OUTPUT_NAME "${OUTPUT_NAME}.opt")
endif()
if(REX_BUILD_TYPE MATCHES "debug")
    set(OUTPUT_NAME "${OUTPUT_NAME}.debug")
endif()
set_target_properties(rex_engine PROPERTIES OUTPUT_NAME "${OUTPUT_NAME}")

if (OPTIMIZE STREQUAL "size_extra")
	target_compile_definitions(rex_engine PUBLIC
		SIZE_EXTRA
	)
endif()

if (ARCH STREQUAL "x86_32")
	if (MSVC)
		target_compile_options(rex_engine PUBLIC
			/arch:SSE2
		)
	else()
		target_compile_options(rex_engine PUBLIC
			-msse2
			-mfpmath=sse
			-mstackrealign
		)
	endif()
endif()
# Compiler flags
if (MSVC)
	if (DEBUG_SYMBOLS)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Zi /FS")
		target_link_options(rex_engine PUBLIC /DEBUG:FULL)
	else()
		target_link_options(rex_engine PUBLIC /DEBUG:NONE)
	endif()
	if (OPTIMIZE MATCHES "speed")
		target_compile_options(rex_engine PUBLIC -O2)
		target_link_options(rex_engine PUBLIC /OPT:REF)
		if (OPTIMIZE STREQUAL "speed_trace")
			target_link_options(rex_engine PUBLIC /OPT:NOICF)
		endif()
	elseif (OPTIMIZE MATCHES "size")
		target_compile_options(rex_engine PUBLIC -O1)
		target_link_options(rex_engine PUBLIC /OPT:REF)
		if (OPTIMIZE STREQUAL "speed_trace")
			target_compile_definitions(rex_engine PUBLIC SIZE_EXTRA)
		endif()
	else()
		target_compile_options(rex_engine PUBLIC -Od)
	endif()
else()
	if (DEBUG_SYMBOLS)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -gdwarf-4")
		if (REX_DEV_BUILD)
			target_compile_options(rex_engine PUBLIC -g3)
		else()
			target_compile_options(rex_engine PUBLIC -g2)
		endif()
    endif()

	if (OPTIMIZE STREQUAL "speed")
		target_compile_options(rex_engine PUBLIC -O3)
	elseif (OPTIMIZE STREQUAL "speed_trace")
		target_compile_options(rex_engine PUBLIC -O2)
	elseif (OPTIMIZE MATCHES "size")
		target_compile_options(rex_engine PUBLIC -Os)
	elseif (OPTIMIZE STREQUAL "debug")
		target_compile_options(rex_engine PUBLIC -Og)
	elseif (OPTIMIZE STREQUAL "none")
		target_compile_options(rex_engine PUBLIC -O0)
	endif()
endif()

# Preprocessor definitions
target_compile_definitions(rex_engine PUBLIC ${PLATFORM_DEFINES})

# Output binary directory
set_target_properties(rex_engine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

add_subdirectory(core)
add_subdirectory(servers)
add_subdirectory(scene)
if (BUILD_TARGET STREQUAL "editor")
	add_subdirectory(editor)
endif()
add_subdirectory(drivers)
add_subdirectory(platform)
add_subdirectory(main)
if (REX_TESTS)
	add_subdirectory(tests)
endif()
add_subdirectory(modules)


# This is placed here so the gathering of doc paths can be part of CMake iterating through the sub directories
if (EDITOR_BUILD)
	execute_process(
		COMMAND "${_Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/generators/rex_editor_builders.py" doc_data_class_path_builder
		"${CMAKE_SOURCE_DIR}/editor/doc_data_class_path.gen.h" "${DOC_CLASS_PATHS}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)

	file(GLOB DOCS ${CMAKE_SOURCE_DIR}/doc/classes/*.xml)
	set(MODULE_DIRS)
	foreach(DOC_PATH ${DOC_CLASS_PATHS})
		list(FIND MODULE_DIRS ${DOC_PATH} DIR_INDEX)
		if (DIR_INDEX EQUAL -1)
			list(APPEND MODULE_DIRS ${DOC_PATH})
		endif()
	endforeach()
	foreach(DIR ${MODULE_DIRS})
		file(GLOB MODULE_DOCS ${DIR}/*.xml)
		list(APPEND DOCS ${MODULE_DOCS})
	endforeach()

	execute_process(
		COMMAND "${_Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/generators/rex_editor_builders.py" make_doc_header
		"${CMAKE_SOURCE_DIR}/editor/doc_data_compressed.gen.h" "${DOCS}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	file(GLOB EDITOR_TRANSLATIONS ${CMAKE_SOURCE_DIR}/editor/translations/editor/*)
	execute_process(
		COMMAND "${_Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/generators/rex_editor_builders.py" make_translations_header
		"${CMAKE_SOURCE_DIR}/editor/editor_translations.gen.h" "${EDITOR_TRANSLATIONS}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	file(GLOB PROPERTIES_TRANSLATIONS ${CMAKE_SOURCE_DIR}/editor/translations/properties/*)
	execute_process(
		COMMAND "${_Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/generators/rex_editor_builders.py" make_translations_header
		"${CMAKE_SOURCE_DIR}/editor/property_translations.gen.h" "${PROPERTIES_TRANSLATIONS}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	file(GLOB DOC_TRANSLATIONS ${CMAKE_SOURCE_DIR}/doc/translations/*)
	execute_process(
		COMMAND "${_Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/generators/rex_editor_builders.py" make_translations_header
		"${CMAKE_SOURCE_DIR}/editor/doc_translations.gen.h" "${DOC_TRANSLATIONS}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	file(GLOB EXTRACTABLE_TRANSLATIONS ${CMAKE_SOURCE_DIR}/editor/translations/extractable/*)
	execute_process(
		COMMAND "${_Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/generators/rex_editor_builders.py" make_translations_header
		"${CMAKE_SOURCE_DIR}/editor/extractable_translations.gen.h" "${EXTRACTABLE_TRANSLATIONS}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
endif()

target_link_libraries(rex_engine PUBLIC
	rex_platform
	rex_drivers
)
if (BUILD_TARGET STREQUAL "editor")
	target_link_libraries(rex_engine PUBLIC rex_editor)
endif()
target_link_libraries(rex_engine PUBLIC
	rex_scene
	rex_servers
	rex_core
	${BUILTIN_LIBRARIES}
	${SYSTEM_LIBRARIES}
)
