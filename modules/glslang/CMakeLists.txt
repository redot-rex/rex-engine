if (VULKAN OR D3D12 OR METAL)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)

	if (BUILTIN_GLSLANG)
		set(THIRDPARTY_GLSLANG_DIR ${CMAKE_SOURCE_DIR}/thirdparty/glslang/)
		add_library(glslang OBJECT
			${THIRDPARTY_GLSLANG_DIR}glslang/GenericCodeGen/CodeGen.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/GenericCodeGen/Link.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/attribute.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/Constant.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/glslang_tab.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/InfoSink.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/Initialize.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/Intermediate.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/intermOut.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/IntermTraverse.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/iomapper.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/limits.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/linkValidate.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/parseConst.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/ParseContextBase.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/ParseHelper.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/PoolAlloc.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/preprocessor/PpAtom.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/preprocessor/PpContext.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/preprocessor/Pp.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/preprocessor/PpScanner.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/preprocessor/PpTokens.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/propagateNoContraction.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/reflection.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/RemoveTree.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/Scan.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/ShaderLang.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/SpirvIntrinsics.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/SymbolTable.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/MachineIndependent/Versions.cpp
			${THIRDPARTY_GLSLANG_DIR}glslang/ResourceLimits/ResourceLimits.cpp
			${THIRDPARTY_GLSLANG_DIR}SPIRV/disassemble.cpp
			${THIRDPARTY_GLSLANG_DIR}SPIRV/doc.cpp
			${THIRDPARTY_GLSLANG_DIR}SPIRV/GlslangToSpv.cpp
			${THIRDPARTY_GLSLANG_DIR}SPIRV/InReadableOrder.cpp
			${THIRDPARTY_GLSLANG_DIR}SPIRV/Logger.cpp
			${THIRDPARTY_GLSLANG_DIR}SPIRV/SpvBuilder.cpp
			${THIRDPARTY_GLSLANG_DIR}SPIRV/SpvPostProcess.cpp
			${THIRDPARTY_GLSLANG_DIR}SPIRV/SPVRemapper.cpp
			${THIRDPARTY_GLSLANG_DIR}SPIRV/SpvTools.cpp
		)
		if (REX_PLATFORM STREQUAL "windows")
			target_sources(rex_module_${MODULE_NAME} INTERFACE
				${THIRDPARTY_GLSLANG_DIR}glslang/OSDependent/Windows/ossource.cpp
			)
		else()
			target_sources(rex_module_${MODULE_NAME} INTERFACE
				${THIRDPARTY_GLSLANG_DIR}glslang/OSDependent/Unix/ossource.cpp
			)
		endif()

		# Include `#thirdparty` to workaround mismatch between location of `SPIRV` in library source
		# and in installed public headers.
		target_include_directories(rex_module_${MODULE_NAME} INTERFACE
			${THIRDPARTY_GLSLANG_DIR}
			${CMAKE_SOURCE_DIR}/thirdparty/
		)
		target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
			ENABLE_OPT=0
		)
		clone_library(rex_module_${MODULE_NAME} glslang)
		disable_warnings_for_target(glslang)
		target_link_libraries(rex_module_${MODULE_NAME} PUBLIC glslang)
	endif()

	file(GLOB GLSLANG_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${GLSLANG_SOURCES}
	)
endif()
