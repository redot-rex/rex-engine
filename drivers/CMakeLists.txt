file(GLOB DRIVERS_SOURCES *.cpp)

add_library(rex_drivers STATIC ${DRIVERS_SOURCES})

target_include_directories(rex_drivers PRIVATE "$<TARGET_PROPERTY:rex_engine,INTERFACE_INCLUDE_DIRECTORIES>")

get_target_property(ENGINE_DEFINITIONS rex_engine INTERFACE_COMPILE_DEFINITIONS)

target_compile_definitions(rex_drivers PUBLIC ${ENGINE_DEFINITIONS})

add_subdirectory(unix)
add_subdirectory(windows)
add_subdirectory(alsa)
add_subdirectory(pulseaudio)
add_subdirectory(alsamidi)
add_subdirectory(winmidi)
add_subdirectory(png)

if (VULKAN)
	add_subdirectory(vulkan)
endif()

if (OPENGL3)
	add_subdirectory(gl_context)
	add_subdirectory(gles3)
	add_subdirectory(egl)
endif()

if (REX_PLATFORM STREQUAL "ios"
	OR REX_PLATFORM STREQUAL "macos")
	add_subdirectory(coreaudio)
	add_subdirectory(apple)
	add_subdirectory(coremidi)

	if (METAL)
		add_subdirectory(metal)
	endif()
endif()


if (REX_PLATFORM STREQUAL "windows")
	add_subdirectory(wasapi)
	if (XAUDIO2)
		add_subdirectory(xaudio2)
	endif()
	if (D3D12)
		add_subdirectory(d3d12)
	endif()
	if (NOT MSVC)
		add_subdirectory(backtrace)
	endif()
endif()

target_link_libraries(rex_engine rex_drivers)
