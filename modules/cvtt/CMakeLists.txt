if (EDITOR_BUILD)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)

	set(THIRDPARTY_CVTT_DIR ${CMAKE_SOURCE_DIR}/thirdparty/cvtt/)

	add_library(cvtt OBJECT
		${THIRDPARTY_CVTT_DIR}ConvectionKernels_API.cpp
		${THIRDPARTY_CVTT_DIR}ConvectionKernels_ETC.cpp
		${THIRDPARTY_CVTT_DIR}ConvectionKernels_BC67.cpp
		${THIRDPARTY_CVTT_DIR}ConvectionKernels_IndexSelector.cpp
		${THIRDPARTY_CVTT_DIR}ConvectionKernels_BC6H_IO.cpp
		${THIRDPARTY_CVTT_DIR}ConvectionKernels_S3TC.cpp
		${THIRDPARTY_CVTT_DIR}ConvectionKernels_BC7_PrioData.cpp
		${THIRDPARTY_CVTT_DIR}ConvectionKernels_SingleFile.cpp
		${THIRDPARTY_CVTT_DIR}ConvectionKernels_BCCommon.cpp
		${THIRDPARTY_CVTT_DIR}ConvectionKernels_Util.cpp
	)

	target_include_directories(rex_module_${MODULE_NAME} INTERFACE
		${THIRDPARTY_CVTT_DIR}
	)

	clone_library(rex_module_${MODULE_NAME} cvtt)
	disable_warnings_for_target(cvtt)
	target_link_libraries(rex_module_${MODULE_NAME} PUBLIC cvtt)
	file(GLOB_RECURSE CVTT_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${CVTT_SOURCES}
	)
endif()
