find_package(PkgConfig REQUIRED)

option(USE_STATIC_CPP "Link libgcc and libstdc++ statically for better portability" ON)
option(USE_COVERAGE "Test Redot coverage" OFF)
option(USE_UBSAN "Use LLVM/GCC compiler undefined behavior sanitizer (UBSAN)" OFF)
option(USE_ASAN "Use LLVM/GCC compiler address sanitizer (ASAN)" OFF)
option(USE_LSAN "Use LLVM/GCC compiler leak sanitizer (LSAN)" OFF)
option(USE_TSAN "Use LLVM/GCC compiler thread sanitizer (TSAN)" OFF)
option(USE_MSAN "Use LLVM compiler memory sanitizer (MSAN)" OFF)
option(USE_SOWRAP "Dynamically load system libraries" ON)
option(ALSA "Use ALSA" ON)
option(PULSEAUDIO "Use PulseAudio" ON)
option(DBUS "Use D-Bus to handle screensaver and portal desktop settings" ON)
option(SPEECHD "Use Speech Dispatcher for Text-to-Speech support" ON)
option(FONTCONFIG "Use fontconfig for system fonts support" ON)
option(UDEV "Use udev for gamepad connection callbacks" ON)
option(X11 "Enable X11 display" ON)
option(WAYLAND "Enable Wayland display" ON)
option(LIBDECOR "Enable libdecor support" ON)
option(TOUCHEVENTS "Enable touch events" ON)
option(EXECINFO "Use libexecinfo on systems where glibc is not available" OFF)

add_executable(rex_engine
	${CMAKE_CURRENT_SOURCE_DIR}/crash_handler_linuxbsd.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/os_linuxbsd.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/joypad_linux.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/freedesktop_portal_desktop.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/freedesktop_screensaver.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/freedesktop_at_spi_monitor.cpp
)

if (USE_SOWRAP)
	target_compile_definitions(rex_engine PUBLIC
		SOWRAP_ENABLED
	)
	target_sources(rex_engine PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/xkbcommon-so_wrap.c
	)
endif()

if (TOUCHEVENTS)
	target_compile_definitions(rex_engine PUBLIC
		TOUCH_ENABLED
	)
endif()

if (FONTCONFIG)
	target_compile_definitions(rex_engine PUBLIC
		FONTCONFIG_ENABLED
	)
endif()

if (ALSA)
	target_compile_definitions(rex_engine PUBLIC
		ALSA_ENABLED
		ALSAMIDI_ENABLED
	)
endif()

if (PULSEAUDIO)
	target_compile_definitions(rex_engine PUBLIC
		PULSEAUDIO_ENABLED
	)
	if (USE_SOWRAP)
		target_compile_definitions(rex_engine PUBLIC
			_REENTRANT
		)
	endif()
endif()

if (DBUS)
	target_compile_definitions(rex_engine PUBLIC
		DBUS_ENABLED
	)
endif()

if (SPEECHD)
	target_compile_definitions(rex_engine PUBLIC
		SPEECHD_ENABLED
	)
endif()

if (USE_SOWRAP)
	target_compile_definitions(rex_engine PUBLIC
		XKB_ENABLED
	)
endif()

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	target_compile_definitions(rex_engine PUBLIC
		JOYDEV_ENABLED
	)
	if (UDEV)
		target_compile_definitions(rex_engine PUBLIC
			UDEV_ENABLED
		)
	endif()
else()
	unset(UDEV)
endif()

if (X11)
	add_subdirectory(x11)
endif()

if (WAYLAND)
	add_subdirectory(wayland)
endif()

if (SPEECHD)
	target_sources(rex_engine PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/tts_linux.cpp
	)
	if (USE_SOWRAP)
		target_sources(rex_engine PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/speechd-so_wrap.c
		)
	endif()
endif()

if (FONTCONFIG)
	if (USE_SOWRAP)
		target_sources(rex_engine PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/fontconfig-so_wrap.c
		)
	endif()

	if (UDEV)
		if (USE_SOWRAP)
			target_sources(rex_engine PRIVATE
				${CMAKE_CURRENT_SOURCE_DIR}/libudev-so_wrap.c
			)
		endif()
	endif()

	if (DBUS)
		if (USE_SOWRAP)
			target_sources(rex_engine PRIVATE
				${CMAKE_CURRENT_SOURCE_DIR}/dbus-so_wrap.c
			)
		endif()
	endif()
endif()
