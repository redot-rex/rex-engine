	file(GLOB PLATFORM_SOURCES
		linuxbsd/api/*.cpp
		windows/api/*.cpp
		macos/api/*.cpp
		android/api/*.cpp
		ios/api/*.cpp
		web/api/*.cpp
	)

add_library(rex_platform STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/register_platform_apis.gen.cpp
	${PLATFORM_SOURCES}
)


clone_library(rex_engine rex_platform)
