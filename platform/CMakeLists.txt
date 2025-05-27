if(REX_PLATFORM STREQUAL "linuxbsd")
	file(GLOB_RECURSE PLATFORM_SOURCES linuxbsd/*.cpp)
elseif(REX_PLATFORM STREQUAL "windows")
	file(GLOB_RECURSE PLATFORM_SOURCES windows/*.cpp)
elseif(REX_PLATFORM STREQUAL "macos")
	file(GLOB_RECURSE PLATFORM_SOURCES macos/*.cpp)
endif()

add_library(rex_platform STATIC
	${PLATFORM_SOURCES}
)

if(REX_PLATFORM STREQUAL "linuxbsd")
	target_include_directories(rex_platform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/linuxbsd/)
elseif(REX_PLATFORM STREQUAL "windows")
	target_include_directories(rex_platform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/windows/)
elseif(REX_PLATFORM STREQUAL "macos")
	target_include_directories(rex_platform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/macos/)
endif()

target_include_directories(rex_platform PRIVATE "$<TARGET_PROPERTY:rex_engine,INTERFACE_INCLUDE_DIRECTORIES>")

get_target_property(ENGINE_DEFINITIONS rex_engine INTERFACE_COMPILE_DEFINITIONS)

target_compile_definitions(rex_platform PUBLIC ${ENGINE_DEFINITIONS})

target_link_libraries(rex_engine rex_platform)
