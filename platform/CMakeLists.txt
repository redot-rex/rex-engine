list(APPEND PLATFORM_APIS android ios web)

set(API_INC "")
set(API_REG "")
set(API_UNREG "")
foreach (PLATFORM_API ${PLATFORM_APIS})
	string(APPEND API_INC "#include \"${PLATFORM_API}/api/api.h\"\n")
	string(APPEND API_REG "\tregister_${PLATFORM_API}_api()\;\n")
	string(APPEND API_UNREG "\tunregister_${PLATFORM_API}_api()\;\n")
endforeach()

generate_copyright_header("register_platform_apis.gen.cpp")
set(REGISTER_API_FILE_CONTENT ${COPYRIGHT_HEADER})
string(APPEND REGISTER_API_FILE_CONTENT "\
#include \"register_platform_apis.h\"

${API_INC}

void register_platform_apis() {
${API_REG}
}

void unregister_platform_apis() {
${API_UNREG}
}"
)

file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/register_platform_apis.gen.cpp ${REGISTER_API_FILE_CONTENT})

file(GLOB PLATFORM_SOURCES
	linuxbsd/api/*.cpp
	windows/api/*.cpp
	macos/api/*.cpp
	android/api/*.cpp
	ios/api/*.cpp
	web/api/*.cpp
)

add_library(rex_platform STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/register_platform_apis.gen.cpp
	${PLATFORM_SOURCES}
)


clone_library(rex_engine rex_platform)
