file(RELATIVE_PATH CURRENT_RELATIVE_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(PLATFORM_DOC_CLASSES "EditorExportPlatformWindows")
set(PLATFORM_DOC_CLASS_PATH "${CURRENT_RELATIVE_PATH}/doc_classes")

foreach(DOC_CLASS ${PLATFORM_DOC_CLASSES})
	list(APPEND DOC_CLASS_PATHS "${DOC_CLASS},${PLATFORM_DOC_CLASS_PATH}")
endforeach()

set(DOC_CLASS_PATHS ${DOC_CLASS_PATHS} PARENT_SCOPE)

if(REX_PLATFORM STREQUAL "windows")
	set(WINDOWS_SUBSYSTEM "gui" CACHE STRING "Windows subsystem: gui, console")
	option(USE_MINGW "Use the Mingw compiler, even if MSVC is installed." OFF)
	option(USE_LLVM "Use the LLVM compiler" OFF)
	option(USE_STATIC_CPP "Link MinGW/MSVC C++ runtime libraries statically" ON)
	option(USE_ASAN "Use address sanitizer (ASAN)" OFF)
	option(USE_UBSAN "Use LLVM compiler undefined behavior sanitizer (UBSAN)" OFF)
	option(DEBUG_CRT "Compile with MSVC's debug CRT (/MDd)" OFF)
	option(INCREMENTAL_LINK "Use MSVC incremental linking. May increase or decrease build times." OFF)
	option(SILENCE_MSVC "Silence MSVC's cl/link stdout bloat, redirecting any errors to stderr." ON)

	if (MSYS)
		set(WINDOWS_CRASH_HANDLER ${CMAKE_CURRENT_SOURCE_DIR}/crash_handler_windows_seh.cpp)
	else()
		set(WINDOWS_CRASH_HANDLER ${CMAKE_CURRENT_SOURCE_DIR}/crash_handler_windows_signal.cpp)
	endif()

	add_executable(rex_engine
		${CMAKE_CURRENT_SOURCE_DIR}/godot_windows.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/os_windows.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/display_server_windows.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/key_mapping_windows.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/joypad_windows.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/tts_windows.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/windows_terminal_logger.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/windows_utils.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/native_menu_windows.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/gl_manager_windows_native.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/gl_manager_windows_angle.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/wgl_detect_version.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/rendering_context_driver_vulkan_windows.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/drop_target_windows.cpp
		${WINDOWS_CRASH_HANDLER}
	)


endif()
