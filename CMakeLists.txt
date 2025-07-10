cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

project(rex_engine
        VERSION 4.3
        DESCRIPTION "Rex Game Engine"
        LANGUAGES C CXX
)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if (MSVC)
	add_compile_options(/std:c17 /std:c++17 /permissive- /Zc:__cplusplus)
else()
	add_compile_options(
		"$<$<COMPILE_LANGUAGE:C>:-std=gnu17>"
		"$<$<COMPILE_LANGUAGE:CXX>:-std=gnu++17>"
	)
endif()

include(methods.cmake)
include(version.cmake)

# TODO: Only checking for desktop platforms for now.
# NOTE: Using strings that make the Scons build system.
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux|DragonFly|FreeBSD|NetBSD|OpenBSD")
	set(REX_PLATFORM "linuxbsd")
	execute_process( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCH )
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
	set(REX_PLATFORM "macos")
	execute_process( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCH )
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	set(REX_PLATFORM "windows")
	execute_process( COMMAND wmic os get osarchitecture COMMAND tr -d '\n' OUTPUT_VARIABLE ARCH )
else()
	message(FATAL_ERROR "${CMAKE_HOST_SYSTEM_NAME} not supported.")
endif()

message(STATUS "Detected build platform: ${REX_PLATFORM}")
message(STATUS "Architecture: ${ARCH}")

include(options.cmake)

if (DEBUG_SYMBOLS)
	set(CMAKE_BUILD_TYPE Debug)
else()
	set(CMAKE_BUILD_TYPE Release)
endif()


# Executable is defined per platform

#if (REX_PLATFORM STREQUAL "linuxbsd")
add_subdirectory(platform/linuxbsd)
	#elseif(REX_PLATFORM STREQUAL "macos")
	#add_subdirectory(platform/macos)
	#elseif(REX_PLATFORM STREQUAL "windows")
add_subdirectory(platform/windows)
	#endif()

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
	add_custom_command(
		OUTPUT ${CMAKE_SOURCE_DIR}/editor/doc_data_class_path.gen.h
		COMMAND ${_Python3_EXECUTABLE}
		ARGS ${CMAKE_SOURCE_DIR}/generators/rex_editor_builders.py doc_data_class_path_builder
		${CMAKE_SOURCE_DIR}/editor/doc_data_class_path.gen.h ${DOC_CLASS_PATHS}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Generating Doc Data Class Path Header..."
	)
	add_custom_target(doc_data_class_path DEPENDS ${CMAKE_SOURCE_DIR}/editor/doc_data_class_path.gen.h)
	add_dependencies(rex_editor doc_data_class_path)

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

	add_custom_command(
		OUTPUT ${CMAKE_SOURCE_DIR}/editor/doc_data_compressed.gen.h
		COMMAND ${_Python3_EXECUTABLE}
		ARGS ${CMAKE_SOURCE_DIR}/generators/rex_editor_builders.py make_doc_header
		${CMAKE_SOURCE_DIR}/editor/doc_data_compressed.gen.h ${DOCS}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Generating Doc Data Compressed Header..."
	)
	add_custom_target(doc_data_compressed DEPENDS ${CMAKE_SOURCE_DIR}/editor/doc_data_compressed.gen.h)
	add_dependencies(rex_editor doc_data_compressed)

	file(GLOB EDITOR_TRANSLATIONS ${CMAKE_SOURCE_DIR}/editor/translations/editor/*)
	add_custom_command(
		OUTPUT ${CMAKE_SOURCE_DIR}/editor/editor_translations.gen.h
		COMMAND ${_Python3_EXECUTABLE}
		ARGS ${CMAKE_SOURCE_DIR}/generators/rex_editor_builders.py make_translations_header
		${CMAKE_SOURCE_DIR}/editor/editor_translations.gen.h ${EDITOR_TRANSLATIONS}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Generating Editor Translations Header..."
	)
	add_custom_target(editor_translations DEPENDS ${CMAKE_SOURCE_DIR}/editor/editor_translations.gen.h)
	add_dependencies(rex_editor editor_translations)
	file(GLOB PROPERTIES_TRANSLATIONS ${CMAKE_SOURCE_DIR}/editor/translations/properties/*)
	add_custom_command(
		OUTPUT ${CMAKE_SOURCE_DIR}/editor/property_translations.gen.h
		COMMAND ${_Python3_EXECUTABLE}
		ARGS ${CMAKE_SOURCE_DIR}/generators/rex_editor_builders.py make_translations_header
		${CMAKE_SOURCE_DIR}/editor/property_translations.gen.h ${PROPERTIES_TRANSLATIONS}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Generating Properties Translations Header..."
	)
	add_custom_target(properties_translations DEPENDS ${CMAKE_SOURCE_DIR}/editor/property_translations.gen.h)
	add_dependencies(rex_editor properties_translations)
	file(GLOB DOC_TRANSLATIONS ${CMAKE_SOURCE_DIR}/doc/translations/*)
	add_custom_command(
		OUTPUT ${CMAKE_SOURCE_DIR}/editor/doc_translations.gen.h
		COMMAND ${_Python3_EXECUTABLE}
		ARGS ${CMAKE_SOURCE_DIR}/generators/rex_editor_builders.py make_translations_header
		${CMAKE_SOURCE_DIR}/editor/doc_translations.gen.h ${DOC_TRANSLATIONS}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Generating Doc Translations Header..."
	)
	add_custom_target(doc_translations DEPENDS ${CMAKE_SOURCE_DIR}/editor/doc_translations.gen.h)
	add_dependencies(rex_editor doc_translations)
	file(GLOB EXTRACTABLE_TRANSLATIONS ${CMAKE_SOURCE_DIR}/editor/translations/extractable/*)
	add_custom_command(
		OUTPUT ${CMAKE_SOURCE_DIR}/editor/extractable_translations.gen.h
		COMMAND ${_Python3_EXECUTABLE}
		ARGS ${CMAKE_SOURCE_DIR}/generators/rex_editor_builders.py make_translations_header
		${CMAKE_SOURCE_DIR}/editor/extractable_translations.gen.h ${EXTRACTABLE_TRANSLATIONS}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Generating Extractable Translations Header..."
	)
	add_custom_target(extractable_translations DEPENDS ${CMAKE_SOURCE_DIR}/editor/extractable_translations.gen.h)
	add_dependencies(rex_editor extractable_translations)

	# Editor Icons
	file(GLOB EDITOR_ICONS ${CMAKE_SOURCE_DIR}/editor/icons/*.svg)
	foreach(MODULE_ICON_PATH ${ICON_PATHS})
		file(GLOB MODULE_ICONS ${MODULE_ICON_PATH}/*.svg)
		list(APPEND EDITOR_ICONS ${MODULE_ICONS})
	endforeach()
	add_custom_command(
		OUTPUT ${CMAKE_SOURCE_DIR}/editor/themes/editor_icons.gen.h
		COMMAND ${_Python3_EXECUTABLE}
		ARGS ${CMAKE_SOURCE_DIR}/generators/rex_editor_icons_builders.py make_editor_icons_action
		${CMAKE_SOURCE_DIR}/editor/themes/editor_icons.gen.h ${EDITOR_ICONS}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Generating Editor Icons Header..."
	)
	add_custom_target(editor_icons DEPENDS ${CMAKE_SOURCE_DIR}/editor/themes/editor_icons.gen.h)
	add_dependencies(rex_editor editor_icons)

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
