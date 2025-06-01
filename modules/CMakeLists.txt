add_library(rex_modules STATIC ${CMAKE_CURRENT_SOURCE_DIR}/register_module_types.gen.cpp)

clone_library(rex_engine rex_modules)
#get_target_property(ENGINE_DEFINITIONS rex_engine INTERFACE_COMPILE_DEFINITIONS)

target_link_libraries(rex_engine PUBLIC rex_modules)
# Modules are added dynamically like it was in Scons
# Only checking for CMakeLists.txt to find modules

generate_copyright_header("modules_enabled.gen.h")
set(MODULE_ENABLED_FILE_CONTENT ${COPYRIGHT_HEADER})
string(APPEND MODULE_ENABLED_FILE_CONTENT "#pragma once\n\n")

generate_copyright_header("register_module_types.gen.cpp")
set(REGISTER_MODULE_FILE_CONTENT ${COPYRIGHT_HEADER})
string(APPEND REGISTER_MODULE_FILE_CONTENT "\
#include \"register_module_types.h\"

#include \"modules/modules_enabled.gen.h\"
")

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
set(MODULE_COMPILE_DEFINES "")
set(INIT_MODULES "")
set(UNINIT_MODULES "")
foreach(MODULE_NAME ${ENABLED_MODULES})
	add_subdirectory("${CMAKE_SOURCE_DIR}/modules/${MODULE_NAME}")
	if (MODULE_IS_ENABLED)
		string(TOUPPER "${MODULE_NAME}" MODULE_UPPER)
		list(APPEND MODULE_COMPILE_DEFINES "MODULE_${MODULE_UPPER}_ENABLED")
		clone_library(rex_modules rex_module_${MODULE_NAME})
		disable_warnings_for_target(rex_module_${MODULE_NAME})
		#target_include_directories(rex_module_${MODULE_NAME} PRIVATE "$<TARGET_PROPERTY:rex_engine,INTERFACE_INCLUDE_DIRECTORIES>")
		#get_target_property(ENGINE_DEFINITIONS rex_engine INTERFACE_COMPILE_DEFINITIONS)
		target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE ${ENGINE_DEFINITIONS})
		target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
			GODOT_MODULE
			${MODULE_COMPILE_DEFINES}
		)
		target_link_libraries(rex_engine PUBLIC rex_module_${MODULE_NAME})
		string(APPEND MODULE_ENABLED_FILE_CONTENT "#define MODULE_${MODULE_UPPER}_ENABLED\n")
		string(APPEND REGISTER_MODULE_FILE_CONTENT "#include \"modules/${MODULE_NAME}/register_types.h\"\n")
		string(APPEND INIT_MODULES "\
#ifdef MODULE_${MODULE_UPPER}_ENABLED
	initialize_${MODULE_NAME}_module(p_level)\;
#endif
")
		string(APPEND UNINIT_MODULES "\
#ifdef MODULE_${MODULE_UPPER}_ENABLED
	uninitialize_${MODULE_NAME}_module(p_level)\;
#endif
")
	endif()
	unset(MODULE_IS_ENABLED)
endforeach()

string(APPEND REGISTER_MODULE_FILE_CONTENT "\

void initialize_modules(ModuleInitializationLevel p_level) {
${INIT_MODULES}
}

void uninitialize_modules(ModuleInitializationLevel p_level) {
${UNINIT_MODULES}
}
")

file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/modules_enabled.gen.h ${MODULE_ENABLED_FILE_CONTENT})
file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/register_module_types.gen.cpp ${REGISTER_MODULE_FILE_CONTENT})

set(BUILTIN_LIBRARIES ${BUILTIN_LIBRARIES} PARENT_SCOPE)
