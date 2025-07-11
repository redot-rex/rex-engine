file(RELATIVE_PATH CURRENT_RELATIVE_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(MODULE_DOC_CLASSES
	RegEx
	RegExMatch
)
set(MODULE_DOC_CLASS_PATH "${CURRENT_RELATIVE_PATH}/doc_classes")

foreach(DOC_CLASS ${MODULE_DOC_CLASSES})
	list(APPEND DOC_CLASS_PATHS "${DOC_CLASS},${MODULE_DOC_CLASS_PATH}")
endforeach()

set(DOC_CLASS_PATHS ${DOC_CLASS_PATHS} PARENT_SCOPE)
list(APPEND ICON_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/icons)
set(ICON_PATHS ${ICON_PATHS} PARENT_SCOPE)

add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

if (BUILTIN_PCRE2)
	set(THIRDPARTY_PCRE2_DIR ${CMAKE_SOURCE_DIR}/thirdparty/pcre2/src/)
	set(PCRE2_SOURCES
        ${THIRDPARTY_PCRE2_DIR}pcre2_auto_possess.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_chartables.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_chkdint.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_compile.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_compile_class.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_config.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_context.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_convert.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_dfa_match.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_error.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_extuni.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_find_bracket.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_jit_compile.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_maketables.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_match.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_match_data.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_newline.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_ord2utf.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_pattern_info.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_script_run.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_serialize.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_string_utils.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_study.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_substitute.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_substring.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_tables.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_ucd.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_valid_utf.c
        ${THIRDPARTY_PCRE2_DIR}pcre2_xclass.c
	)
	set(PCRE2_FLAGS
		PCRE2_STATIC
		HAVE_CONFIG_H
		SUPPORT_UNICODE
	)
	if (BUILTIN_PCRE2_WITH_JIT)
		list(APPEND PCRE2_FLAGS SUPPORT_JIT)
	endif()

	foreach(WIDTH 16 32)
		add_library(pcre2_${WIDTH} STATIC ${PCRE2_SOURCES})
		target_include_directories(pcre2_${WIDTH} PRIVATE ${THIRDPARTY_PCRE2_DIR})
		target_compile_definitions(pcre2_${WIDTH} PRIVATE
			${PCRE2_FLAGS}
			PCRE2_CODE_UNIT_WIDTH=${WIDTH}
		)
		target_link_libraries(rex_module_${MODULE_NAME} PRIVATE pcre2_${WIDTH})
	endforeach()

	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${THIRDPARTY_PCRE2_DIR}
	)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		${PCRE2_FLAGS}
		PCRE2_CODE_UNIT_WIDTH=0
	)
endif()

file(GLOB REGEX_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${REGEX_SOURCES}
)
