add_library(rex_modules STATIC ${CMAKE_CURRENT_SOURCE_DIR}/register_module_types.gen.cpp)

target_include_directories(rex_modules PRIVATE "$<TARGET_PROPERTY:rex_engine,INTERFACE_INCLUDE_DIRECTORIES>")

get_target_property(ENGINE_DEFINITIONS rex_engine INTERFACE_COMPILE_DEFINITIONS)

target_compile_definitions(rex_modules PUBLIC ${ENGINE_DEFINITIONS})

# Modules are added dynamically like it was in Scons
# Only checking for CMakeLists.txt to find modules

generate_copyright_header("modules_enabled.gen.h")
set(MODULE_ENABLED_FILE_CONTENT ${COPYRIGHT_HEADER})
string(APPEND MODULE_ENABLED_FILE_CONTENT "#pragma once\n\n")

generate_copyright_header("register_module_types.gen.cpp")
set(REGISTER_MODULE_FILE_CONTENT ${COPYRIGHT_HEADER})
string(APPEND REGISTER_MODULE_FILE_CONTENT "\
#include \"register_module_types.h\"\n\
\n\
#include \"modules/modules_enabled.gen.h\"\n\
\n"
)

file(GLOB MODULE_DIRS RELATIVE ${CMAKE_SOURCE_DIR}/modules ${CMAKE_SOURCE_DIR}/modules/*)
set(ENABLED_MODULES "")
set(MODULE_COMPILE_DEFINES "")
set(INIT_MODULES "")
set(UNINIT_MODULES "")
foreach(MODULE_DIR ${MODULE_DIRS})
	if (IS_DIRECTORY "${CMAKE_SOURCE_DIR}/modules/${MODULE_DIR}")
		if (EXISTS "${CMAKE_SOURCE_DIR}/modules/${MODULE_DIR}/CMakeLists.txt")
			string(TOUPPER "${MODULE_DIR}" MODULE_UPPER)
			list(APPEND MODULE_COMPILE_DEFINES "MODULE_${MODULE_UPPER}_ENABLED")
			list(APPEND ENABLED_MODULES ${MODULE_DIR})
			string(APPEND MODULE_ENABLED_FILE_CONTENT "#define MODULE_${MODULE_UPPER}_ENABLED\n")
			string(APPEND REGISTER_MODULE_FILE_CONTENT "#include \"modules/${MODULE_DIR}/register_types.h\"\n")
			string(APPEND INIT_MODULES "\
#ifdef MODULE_${MODULE_UPPER}_ENABLED\n\
	initialize_${MODULE_DIR}_module(p_level)\;\n\
#endif\n"
)
			string(APPEND UNINIT_MODULES "\
#ifdef MODULE_${MODULE_UPPER}_ENABLED\n\
	uninitialize_${MODULE_DIR}_module(p_level)\;\n\
#endif\n"
)
		endif()
	endif()
endforeach()

string(APPEND REGISTER_MODULE_FILE_CONTENT "
\n\
void initialize_modules(ModuleInitializationLevel p_level) {\n\
${INIT_MODULES}\
}\n\
\n\
void uninitialize_modules(ModuleInitializationLevel p_level) {\n\
${UNINIT_MODULES}\
}\n"
)

file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/modules_enabled.gen.h ${MODULE_ENABLED_FILE_CONTENT})
file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/register_module_types.gen.cpp ${REGISTER_MODULE_FILE_CONTENT})

set(MODULE_SOURCES "")
foreach(MODULE_NAME ${ENABLED_MODULES})
	add_library(rex_module_${MODULE_NAME} STATIC)
	add_subdirectory("${CMAKE_SOURCE_DIR}/modules/${MODULE_NAME}")
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE "$<TARGET_PROPERTY:rex_engine,INTERFACE_INCLUDE_DIRECTORIES>")
	get_target_property(ENGINE_DEFINITIONS rex_engine INTERFACE_COMPILE_DEFINITIONS)
	target_compile_definitions(rex_module_${MODULE_NAME} PUBLIC ${ENGINE_DEFINITIONS})
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		GODOT_MODULE
		${MODULE_COMPILE_DEFINES}
	)
	target_link_libraries(rex_modules rex_module_${MODULE_NAME})
endforeach()

target_link_libraries(rex_engine rex_modules)
