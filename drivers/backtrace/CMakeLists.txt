if(REX_PLATFORM STREQUAL "windows")
	set(THIRDPARTY_BACKTRACE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/libbacktrace/)
	add_library(libbacktrace OBJECT
		${THIRDPARTY_BACKTRACE_DIR}atomic.c
		${THIRDPARTY_BACKTRACE_DIR}dwarf.c
		${THIRDPARTY_BACKTRACE_DIR}fileline.c
		${THIRDPARTY_BACKTRACE_DIR}posix.c
		${THIRDPARTY_BACKTRACE_DIR}print.c
		${THIRDPARTY_BACKTRACE_DIR}sort.c
		${THIRDPARTY_BACKTRACE_DIR}state.c
		${THIRDPARTY_BACKTRACE_DIR}backtrace.c
		${THIRDPARTY_BACKTRACE_DIR}simple.c
		${THIRDPARTY_BACKTRACE_DIR}pecoff.c
		${THIRDPARTY_BACKTRACE_DIR}read.c
		${THIRDPARTY_BACKTRACE_DIR}alloc.c
	)

	target_include_directories(rex_drivers INTERFACE
		${THIRDPARTY_BACKTRACE_DIR}
	)
	clone_library(rex_drivers libbacktrace)
	disable_warnings_for_target(libbacktrace)

	target_link_libraries(rex_drivers backtrace)
endif()
