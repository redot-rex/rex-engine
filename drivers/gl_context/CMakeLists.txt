if (OPENGL3)
	set(THIRDPARTY_GLAD_DIR ${CMAKE_SOURCE_DIR}/thirdparty/glad/)
	add_library(gl OBJECT
		${THIRDPARTY_GLAD_DIR}gl.c
	)
	if (NOT ANGLE_LIBS)
		target_sources(gl PRIVATE
			${THIRDPARTY_GLAD_DIR}egl.c
		)
	endif()

	target_include_directories(rex_engine PUBLIC
		${THIRDPARTY_GLAD_DIR}
	)

	target_compile_definitions(rex_engine PUBLIC
		GLAD_ENABLED
		EGL_ENABLED
	)
	clone_library(rex_engine gl)
	disable_warnings_for_target(gl)
	target_link_libraries(rex_engine PUBLIC gl)

endif()
