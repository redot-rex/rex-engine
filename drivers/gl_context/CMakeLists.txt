if (OPENGL3)
	set(THIRDPARTY_GLAD_DIR ${CMAKE_SOURCE_DIR}/thirdparty/glad/)
	target_sources(rex_drivers PRIVATE
		${THIRDPARTY_GLAD_DIR}gl.c
		# TODO: include egl.c for angle libs
	)

	target_include_directories(rex_engine PUBLIC
		${THIRDPARTY_GLAD_DIR}
	)

	target_compile_definitions(rex_engine PUBLIC
		GLAD_ENABLED
		EGL_ENABLED
	)
endif()
