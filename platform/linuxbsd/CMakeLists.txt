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

if (NOT BUILTIN_FREETYPE)
	pkg_check_modules(FREETYPE2 QUIET freetype2)
	if(FREETYPE2_FOUND)
		target_include_directories(rex_engine PUBLIC ${FREETYPE2_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${FREETYPE2_LIBRARIES})
	else()
		message(WARNING "freetype2 not found.")
	endif()
endif()

if (NOT BUILTIN_GRAPHITE)
	pkg_check_modules(GRAPHITE2 QUIET graphite2)
	if(GRAPHITE2_FOUND)
		target_include_directories(rex_engine PUBLIC ${GRAPHITE2_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${GRAPHITE2_LIBRARIES})
	else()
		message(WARNING "graphite2 not found.")
	endif()
endif()

if (NOT BUILTIN_ICU4C)
	pkg_check_modules(ICU4C QUIET icu-i18n icu-uc)
	if(ICU4C_FOUND)
		target_include_directories(rex_engine PUBLIC ${ICU4C_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${ICU4C_LIBRARIES})
	else()
		message(WARNING "icu4c not found.")
	endif()
endif()

if (NOT BUILTIN_HARFBUZZ)
	pkg_check_modules(HARFBUZZ QUIET harfbuzz harfbuzz-icu)
	if(HARFBUZZ_FOUND)
		target_include_directories(rex_engine PUBLIC ${HARFBUZZ_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${HARFBUZZ_LIBRARIES})
	else()
		message(WARNING "harfbuzz not found.")
	endif()
endif()

if (NOT BUILTIN_ICU4C OR NOT BUILTIN_HARFBUZZ)
    message(WARNING "System-provided icu4c or harfbuzz cause known issues for GDExtension (see GH-91401 and
	GH-100301).")
endif()

if (NOT BUILTIN_LIBPNG)
	pkg_check_modules(LIBPNG QUIET libpng16)
	if(LIBPNG_FOUND)
		target_include_directories(rex_engine PUBLIC ${LIBPNG_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${LIBPNG_LIBRARIES})
	else()
		message(WARNING "libpng16 not found.")
	endif()
endif()

if (NOT BUILTIN_ENET)
	pkg_check_modules(ENET QUIET libenet)
	if(ENET_FOUND)
		target_include_directories(rex_engine PUBLIC ${ENET_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${ENET_LIBRARIES})
	else()
		message(WARNING "libenet not found.")
	endif()
endif()

if (NOT BUILTIN_ZSTD)
	pkg_check_modules(ZSTD QUIET libzstd)
	if(ZSTD_FOUND)
		target_include_directories(rex_engine PUBLIC ${ZSTD_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${ZSTD_LIBRARIES})
	else()
		message(WARNING "libzstd not found.")
	endif()
endif()

if (BROTLI AND NOT BUILTIN_BROTLI)
	pkg_check_modules(BROTLI_LIB QUIET libbrotlicommon libbrotlidec)
	if(BROTLI_LIB_FOUND)
		target_include_directories(rex_engine PUBLIC ${BROTLI_LIB_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${BROTLI_LIB_LIBRARIES})
	else()
		message(WARNING "brotli not found.")
	endif()
endif()

if (NOT BUILTIN_LIBTHEORA)
	set(BUILTIN_LIBOGG OFF)
	set(BUILTIN_LIBVORBIS OFF)
	pkg_check_modules(LIBTHEORA QUIET theora theoradec)
	if(LIBTHEORA_FOUND)
		target_include_directories(rex_engine PUBLIC ${LIBTHEORA_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${LIBTHEORA_LIBRARIES})
	else()
		message(WARNING "theora not found.")
	endif()
	if (ARCH MATCHES "x86_64|x86_32")
		set(X86_LIBTHEORA_OPT_GCC ON)
	endif()
endif()

if (NOT BUILTIN_LIBVORBIS)
	set(BUILTIN_LIBOGG OFF)
	pkg_check_modules(LIBVORBIS QUIET vorbis vorbisfile)
	if(LIBVORBIS_FOUND)
		target_include_directories(rex_engine PUBLIC ${LIBVORBIS_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${LIBVORBIS_LIBRARIES})
	else()
		message(WARNING "vorbis not found.")
	endif()
endif()

if (NOT BUILTIN_LIBOGG)
	pkg_check_modules(LIBOGG QUIET ogg)
	if(LIBOGG_FOUND)
		target_include_directories(rex_engine PUBLIC ${LIBOGG_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${LIBOGG_LIBRARIES})
	else()
		message(WARNING "ogg not found.")
	endif()
endif()

if (NOT BUILTIN_LIBWEBP)
	pkg_check_modules(LIBWEBP QUIET libwebp)
	if(LIBWEBP_FOUND)
		target_include_directories(rex_engine PUBLIC ${LIBWEBP_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${LIBWEBP_LIBRARIES})
	else()
		message(WARNING "libwebp not found.")
	endif()
endif()

if (NOT BUILTIN_MBEDTLS)
	pkg_check_modules(MBEDTLS QUIET mbedtls mbedcrypto mbedx509)
	if(MBEDTLS_FOUND)
		target_include_directories(rex_engine PUBLIC ${MBEDTLS_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${MBEDTLS_LIBRARIES})
	else()
		message(WARNING "mbedtls not found.")
	endif()
endif()

if (NOT BUILTIN_WSLAY)
	pkg_check_modules(WSLAY QUIET libwslay)
	if(WSLAY_FOUND)
		target_include_directories(rex_engine PUBLIC ${WSLAY_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${WSLAY_LIBRARIES})
	else()
		message(WARNING "libwslay not found.")
	endif()
endif()

if (NOT BUILTIN_MINIUPNPC)
	pkg_check_modules(MINIUPNPC QUIET miniupnpc)
	if(MINIUPNPC_FOUND)
		target_include_directories(rex_engine PUBLIC ${MINIUPNPC_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${MINIUPNPC_LIBRARIES})
	else()
		message(WARNING "miniupnpc not found.")
	endif()
endif()

if (NOT BUILTIN_PCRE2)
	pkg_check_modules(PCRE2 QUIET libpcre2-32)
	if(PCRE2_FOUND)
		target_include_directories(rex_engine PUBLIC ${PCRE2_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${PCRE2_LIBRARIES})
	else()
		message(WARNING "libpcre2-32 not found.")
	endif()
endif()

if (NOT BUILTIN_RECASTNAVIGATION)
	find_library(RECAST_LIB Recast)
	if (RECAST_LIB)
		target_include_directories(rex_engine PUBLIC
			/usr/include/recastnavigation/
		)
		target_link_libraries(rex_engine RECAST_LIB)
	endif()
endif()

if (NOT BUILTIN_EMBREE AND ARCH MATCHES "x86_64|arm64")
	find_library(EMBREE_LIB embree4)
	if (EMBREE_LIB)
		target_link_libraries(rex_engine EMBREE_LIB)
	endif()
endif()

if (NOT BUILTIN_OPENXR)
	pkg_check_modules(OPENXR QUIET openxr)
	if(OPENXR_FOUND)
		target_include_directories(rex_engine PUBLIC ${OPENXR_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${OPENXR_LIBRARIES})
	else()
		message(WARNING "openxr not found.")
	endif()
endif()

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
	if (NOT USE_SOWRAP)
		pkg_check_modules(FONTCONFIG_LIB QUIET fontconfig)
		if(FONTCONFIG_LIB_FOUND)
			target_include_directories(rex_engine PUBLIC ${FONTCONFIG_LIB_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${FONTCONFIG_LIB_LIBRARIES})
			target_compile_definitions(rex_engine PUBLIC
				FONTCONFIG_ENABLED
			)
		else()
			message(WARNING "fontconfig development libraries not found. Disabling the system fonts support.")
			unset(FONTCONFIG)
		endif()
	else()
		target_compile_definitions(rex_engine PUBLIC
			FONTCONFIG_ENABLED
		)
	endif()
endif()

if (ALSA)
	if (NOT USE_SOWRAP)
		pkg_check_modules(ALSA_LIB QUIET alsa)
		if(ALSA_LIB_FOUND)
			target_include_directories(rex_engine PUBLIC ${ALSA_LIB_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${ALSA_LIB_LIBRARIES})
			target_compile_definitions(rex_engine PUBLIC
				ALSA_ENABLED
				ALSAMIDI_ENABLED
			)
		else()
			message(WARNING "ALSA development libraries not found. Disabling the ALSA audio driver.")
			unset(ALSA)
		endif()
	else()
		target_compile_definitions(rex_engine PUBLIC
			ALSA_ENABLED
			ALSAMIDI_ENABLED
		)
	endif()
endif()

if (PULSEAUDIO)
	if (NOT USE_SOWRAP)
		pkg_check_modules(PULSEAUDIO_LIB QUIET libpulse)
		if(PULSEAUDIO_LIB_FOUND)
			target_include_directories(rex_engine PUBLIC ${PULSEAUDIO_LIB_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${PULSEAUDIO_LIB_LIBRARIES})
			target_compile_definitions(rex_engine PUBLIC
				PULSEAUDIO_ENABLED
			)
		else()
			message(WARNING "PulseAudio development libraries not found. Disabling the PulseAudio audio driver.")
			unset(PULSEAUDIO)
		endif()
	else()
		target_compile_definitions(rex_engine PUBLIC
			PULSEAUDIO_ENABLED
			_REENTRANT
		)
	endif()
endif()

if (DBUS)
	if (NOT USE_SOWRAP)
		pkg_check_modules(DBUS_LIB QUIET dbus-1)
		if(DBUS_LIB_FOUND)
			target_include_directories(rex_engine PUBLIC ${DBUS_LIB_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${DBUS_LIB_LIBRARIES})
			target_compile_definitions(rex_engine PUBLIC
				DBUS_ENABLED
			)
		else()
			message(WARNING "D-Bus development libraries not found. Disabling screensaver prevention.")
			unset(DBUS)
		endif()
	else()
		target_compile_definitions(rex_engine PUBLIC
			DBUS_ENABLED
		)
	endif()
endif()

if (SPEECHD)
	if (NOT USE_SOWRAP)
		pkg_check_modules(SPEECHD_LIB QUIET speech-dispatcher)
		if(SPEECHD_LIB_FOUND)
			target_include_directories(rex_engine PUBLIC ${SPEECHD_LIB_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${SPEECHD_LIB_LIBRARIES})
			target_compile_definitions(rex_engine PUBLIC
				SPEECHD_ENABLED
			)
		else()
			message(WARNING "speech-dispatcher development libraries not found. Disabling text to speech support.")
			unset(SPEECHD)
		endif()
	else()
		target_compile_definitions(rex_engine PUBLIC
			SPEECHD_ENABLED
		)
	endif()
endif()

if (USE_SOWRAP)
	pkg_check_modules(XKBCOMMON QUIET xkbcommon)
	if(XKBCOMMON_FOUND)
		target_include_directories(rex_engine PUBLIC ${XKBCOMMON_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${XKBCOMMON_LIBRARIES})
		target_compile_definitions(rex_engine PUBLIC
			XKB_ENABLED
		)
	else()
		if (WAYLAND)
			message(FATAL_ERROR "libxkbcommon development libraries required by Wayland not found. Aborting.")
		else()
			message(WARNING "libxkbcommon development libraries not found. Disabling dead key composition and key label support.")
		endif()
	endif()
else()
	target_compile_definitions(rex_engine PUBLIC
		XKB_ENABLED
	)
endif()

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	target_compile_definitions(rex_engine PUBLIC
		JOYDEV_ENABLED
	)
	if (UDEV)
		pkg_check_modules(UDEV_LIB QUIET libudev)
		if(UDEV_LIB_FOUND)
			target_include_directories(rex_engine PUBLIC ${UDEV_LIB_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${UDEV_LIB_LIBRARIES})
			target_compile_definitions(rex_engine PUBLIC
				UDEV_ENABLED
			)
		else()
			message(WARNING "libudev development libraries not found. Disabling controller hotplugging support.")
			unset(UDEV)
		endif()
	endif()
else()
	unset(UDEV)
endif()

if (NOT BUILTIN_ZLIB)
	pkg_check_modules(ZLIB QUIET zlib)
	if(ZLIB_FOUND)
		target_include_directories(rex_engine PUBLIC ${ZLIB_INCLUDE_DIRS})
		target_link_libraries(rex_engine ${ZLIB_LIBRARIES})
	else()
		message(WARNING "zlib not found.")
	endif()
endif()

target_compile_definitions(rex_engine PUBLIC
	LINUXBSD_ENABLED
	UNIX_ENABLED
	_FILE_OFFSET_BITS=64
)
target_include_directories(rex_engine PUBLIC
	${CMAKE_SOURCE_DIR}/platform/linuxbsd/
)
if (USE_SOWRAP)
	target_include_directories(rex_engine PUBLIC
		${CMAKE_SOURCE_DIR}/thirdparty/linuxbsd_headers/
	)
endif()
if (X11)
	if (NOT USE_SOWRAP)
		pkg_check_modules(X11_LIB QUIET x11)
		if (X11_LIB_FOUND)
			target_include_directories(rex_engine PUBLIC ${X11_LIB_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${X11_LIB_LIBRARIES})
		else()
			message(FATAL_ERROR "X11 libraries not found.")
		endif()
		pkg_check_modules(XCURSOR QUIET xcursor)
		if (XCURSOR_FOUND)
			target_include_directories(rex_engine PUBLIC ${XCURSOR_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${XCURSOR_LIBRARIES})
		else()
			message(FATAL_ERROR "Xcursor library not found.")
		endif()
		pkg_check_modules(XINERAMA QUIET xinerama)
		if (XINERAMA_FOUND)
			target_include_directories(rex_engine PUBLIC ${XINERAMA_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${XINERAMA_LIBRARIES})
		else()
			message(FATAL_ERROR "Xinerama library not found.")
		endif()
		pkg_check_modules(XEXT QUIET xext)
		if (XEXT_FOUND)
			target_include_directories(rex_engine PUBLIC ${XEXT_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${XEXT_LIBRARIES})
		else()
			message(FATAL_ERROR "Xext library not found.")
		endif()
		pkg_check_modules(XRANDR QUIET xrandr)
		if (XRANDR_FOUND)
			target_include_directories(rex_engine PUBLIC ${XRANDR_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${XRANDR_LIBRARIES})
		else()
			message(FATAL_ERROR "Xrandr library not found.")
		endif()
		pkg_check_modules(XRENDER QUIET xrender)
		if (XRENDER_FOUND)
			target_include_directories(rex_engine PUBLIC ${XRENDER_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${XRENDER_LIBRARIES})
		else()
			message(FATAL_ERROR "Xrender library not found.")
		endif()
		pkg_check_modules(XI QUIET xi)
		if (XI_FOUND)
			target_include_directories(rex_engine PUBLIC ${XI_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${XI_LIBRARIES})
		else()
			message(FATAL_ERROR "Xi library not found.")
		endif()
	endif()
	target_compile_definitions(rex_engine PUBLIC
		X11_ENABLED
	)
endif()
if (WAYLAND)
	if (NOT USE_SOWRAP)
		pkg_check_modules(LIBDECOR_LIB QUIET libdecor-0)
		if (LIBDECOR_LIB_FOUND)
			target_include_directories(rex_engine PUBLIC ${LIBDECOR_LIB_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${LIBDECOR_LIB_LIBRARIES})
		else()
			message(WARNING "libdecor development libraries not found. Disabling client-side decorations.")
			unset(LIBDECOR)
		endif()
		pkg_check_modules(WAYLAND_CLIENT QUIET wayland-client)
		if (WAYLAND_CLIENT_FOUND)
			target_include_directories(rex_engine PUBLIC ${WAYLAND_CLIENT_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${WAYLAND_CLIENT_LIBRARIES})
		else()
			message(FATAL_ERROR "wayland-client library not found.")
		endif()
		pkg_check_modules(WAYLAND_CURSOR QUIET wayland-cursor)
		if (WAYLAND_CURSOR_FOUND)
			target_include_directories(rex_engine PUBLIC ${WAYLAND_CURSOR_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${WAYLAND_CURSOR_LIBRARIES})
		else()
			message(FATAL_ERROR "wayland-cursor library not found.")
		endif()
		pkg_check_modules(WAYLAND_EGL QUIET wayland-egl)
		if(WAYLAND_EGL_FOUND)
			target_include_directories(rex_engine PUBLIC ${WAYLAND_EGL_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${WAYLAND_EGL_LIBRARIES})
		else()
			message(FATAL_ERROR "wayland-egl library not found.")
		endif()
	else()
		target_include_directories(rex_engine PUBLIC
			${CMAKE_SOURCE_DIR}/thirdparty/linuxbsd_headers/wayland/
		)
		if (LIBDECOR)
			target_include_directories(rex_engine PUBLIC
				${CMAKE_SOURCE_DIR}/thirdparty/linuxbsd_headers/libdecor-0/
			)
		endif()
	endif()

	if (LIBDECOR)
		target_compile_definitions(rex_engine PUBLIC
			LIBDECOR_ENABLED
		)
	endif()

	# Needed by glibc, used by _allocate_shm_file
	find_library(RT_LIB rt)
	if (RT_LIB)
		target_link_libraries(rex_engine RT_LIB)
	endif()

	target_compile_definitions(rex_engine PUBLIC
		WAYLAND_ENABLED
	)
endif()

if (VULKAN)
	target_compile_definitions(rex_engine PUBLIC
		VULKAN_ENABLED
		RD_ENABLED
	)
	if (NOT USE_VOLK)
		pkg_check_modules(VULKAN_LIB QUIET vulkan)
		if(VULKAN_LIB_FOUND)
			target_include_directories(rex_engine PUBLIC ${VULKAN_LIB_INCLUDE_DIRS})
			target_link_libraries(rex_engine ${VULKAN_LIB_LIBRARIES})
		else()
			message(FATAL_ERROR "vulkan library not found.")
		endif()
	endif()
	if (NOT BUILTIN_GLSLANG)
		find_library(GLSLANG glslang)
		if (GLSLANG)
			target_link_libraries(rex_engine GLSLANG)
		endif()
		find_library(SPIRV_LIB SPIRV)
		if (SPIRV_LIB)
			target_link_libraries(rex_engine SPIRV_LIB)
		endif()
	endif()
endif()
if (OPENGL3)
	target_compile_definitions(rex_engine PUBLIC
		GLES3_ENABLED
	)
endif()

find_library(PTHREAD pthread)
if (PTHREAD)
	target_link_libraries(rex_engine PTHREAD)
endif()

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	find_library(DL_LIB dl)
	if (DL_LIB)
		target_link_libraries(rex_engine DL_LIB)
	endif()
endif()

target_compile_definitions(rex_engine PUBLIC
	CRASH_HANDLER_ENABLED
)

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
