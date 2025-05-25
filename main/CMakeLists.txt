file(GLOB_RECURSE MAIN_SOURCES *.cpp)

# 1. Generate splash.gen.h from splash.png
add_custom_command(
  OUTPUT "${CMAKE_SOURCE_DIR}/main/splash.gen.h"
  COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_SOURCE_DIR}/main/main_builders.py" make_splash "${CMAKE_SOURCE_DIR}/main/splash.png"
  DEPENDS "${CMAKE_SOURCE_DIR}/main/splash.png"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/main"
  COMMENT "Generating splash.gen.h"
)
add_custom_target(generate_splash DEPENDS "${CMAKE_SOURCE_DIR}/main/splash.gen.h")

# 2. Generate splash_editor.gen.h from splash_editor.png (conditional)
if(REX_TOOLS)
  add_custom_command(
    OUTPUT "${CMAKE_SOURCE_DIR}/main/splash_editor.gen.h"
    COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_SOURCE_DIR}/main/main_builders.py" make_splash_editor "${CMAKE_SOURCE_DIR}/main/splash_editor.png"
    DEPENDS "${CMAKE_SOURCE_DIR}/main/splash.png"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/main"
    COMMENT "Generating splash_editor.gen.h"
  )
  add_custom_target(generate_splash_editor DEPENDS "${CMAKE_SOURCE_DIR}/main/splash_editor.gen.h")
  #add_dependencies(rex_engine generate_splash_editor)
  #target_sources(rex_engine PRIVATE "${CMAKE_SOURCE_DIR}/main/splash_editor.gen.h")
endif()

# 3. Generate app_icon.gen.h from app_icon.png
add_custom_command(
  OUTPUT "${CMAKE_SOURCE_DIR}/main/app_icon.gen.h"
  COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_SOURCE_DIR}/main/main_builders.py" make_app_icon "${CMAKE_SOURCE_DIR}/main/app_icon.png"
  DEPENDS "${CMAKE_SOURCE_DIR}/main/app_icon.png"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/main"
  COMMENT "Generating app_icon.gen.h"
)
add_custom_target(generate_app_icon DEPENDS "${CMAKE_SOURCE_DIR}/main/app_icon.gen.h")

#add_dependencies(rex_engine generate_splash generate_app_icon)

target_sources(rex_engine PRIVATE
	${MAIN_SOURCES}
	"${CMAKE_SOURCE_DIR}/main/app_icon.gen.h"
	"${CMAKE_SOURCE_DIR}/main/splash.gen.h"
)
