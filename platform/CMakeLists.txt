if(REX_PLATFORM STREQUAL "linuxbsd")
	file(GLOB_RECURSE PLATFORM_SOURCES linuxbsd/api/*.cpp)
elseif(REX_PLATFORM STREQUAL "windows")
	file(GLOB_RECURSE PLATFORM_SOURCES windows/api/*.cpp)
elseif(REX_PLATFORM STREQUAL "macos")
	file(GLOB_RECURSE PLATFORM_SOURCES macos/api/*.cpp)
endif()

add_library(rex_platform STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/register_platform_apis.gen.cpp
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
