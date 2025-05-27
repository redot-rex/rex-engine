add_library(rex_modules STATIC)

target_include_directories(rex_modules PRIVATE "$<TARGET_PROPERTY:rex_engine,INTERFACE_INCLUDE_DIRECTORIES>")

get_target_property(ENGINE_DEFINITIONS rex_engine INTERFACE_COMPILE_DEFINITIONS)

target_compile_definitions(rex_modules PUBLIC ${ENGINE_DEFINITIONS})

# Modules are added dynamically like it was in Scons
# Only checking for CMakeLists.txt to find modules

file(GLOB MODULE_DIRS RELATIVE ${CMAKE_SOURCE_DIR}/modules ${CMAKE_SOURCE_DIR}/modules/*)
set(ENABLED_MODULES "")
foreach(MODULE_DIR ${MODULE_DIRS})
	if (IS_DIRECTORY "${CMAKE_SOURCE_DIR}/modules/${MODULE_DIR}")
		if (EXISTS "${CMAKE_SOURCE_DIR}/modules/${MODULE_DIR}/CMakeLists.txt")
			list(APPEND ENABLED_MODULES ${MODULE_DIR})
		endif()
	endif()
endforeach()

set(MODULE_SOURCES "")
foreach(MODULE_DIR ${ENABLED_MODULES})
	add_subdirectory("${CMAKE_SOURCE_DIR}/modules/${MODULE_DIR}")
endforeach()

target_link_libraries(rex_engine rex_modules)
