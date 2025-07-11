# Build options

set(OPTIMIZE "auto" CACHE STRING "Optimization level (by default inferred from 'target' and 'dev_build'): auto, none,
custom, debug, speed, speed_trace, size, size_extra")
set(BUILD_TARGET "editor" CACHE STRING "Compilation target: editor, template_release, template_debug")

option(REX_TOOLS "Build editor tools" OFF)
option(REX_DEV_BUILD "Enable developer build" OFF)

option(DEBUG_SYMBOLS "Build with debugging symbols" OFF)
option(SEPARATE_DEBUG_SYMBOLS "Extract debugging symbols to a separate file" OFF)
option(DEBUG_RELATIVE_PATHS "Make file paths in debug symbols relative (if supported)" OFF)

set(LTO "none" CACHE STRING "Link-time optimization (production builds): none, auto, thin, full")

option(PRODUCTION "Set defaults to build Redot for use in production" OFF)
option(THREADS "Enable Threading Support" ON)

# Components

option(DEPRICATED "Enable compatibility code of deprecated and removed features" ON)
set(PRECISION "single" CACHE STRING "Set the floating-point precision level: single, double")
option(MINIZIP "Enable ZIP archive support using minizip" ON)
option(BROTLI "Enable Brotli for decompression and WOFF2 fonts support" ON)
option(XAUDIO2 "Enable XAudio2 audio driver on supported platforms" OFF)
option(VULKAN "Enable the vulkan rendering driver" ON)
option(OPENGL3 "Enable the OpenGL/GLES3 rendering driver" ON)
option(D3D12 "Enable the Direct3D 12 rendering driver on supported platforms" OFF)
option(METAL "Enable the Metal rendering driver on supported platforms (Apple arm64 only)" OFF)
option(USE_VOLK "Use the volk library to load the Vulkan loader dynamically" ON)
option(DISABLE_EXCEPTIONS "Force disabling exception handling code" ON)

# Advanced options
option(REX_TESTS "Build the unit tests" OFF)

# Third Party options

option(BUILTIN_BROTLI "Use the built-in Brotli library" ON)
option(BUILTIN_CERTS "Use the built-in SSL certificates bundles" ON)
option(BUILTIN_CLIPPER2 "Use the built-in Clipper2 library" ON)
option(BUILTIN_EMBREE "Use the built-in Embree library" ON)
option(BUILTIN_ENET "Use the built-in ENet library" ON)
option(BUILTIN_FREETYPE "Use the built-in FreeType library" ON)
option(BUILTIN_MSDFGEN "Use the built-in MSDFgen library" ON)
option(BUILTIN_GLSLANG "Use the built-in glslang library" ON)
option(BUILTIN_GRAPHITE "Use the built-in Graphite library" ON)
option(BUILTIN_HARFBUZZ "Use the built-in HarfBuzz library" ON)
option(BUILTIN_ICU4C "Use the built-in ICU library" ON)
option(BUILTIN_LIBOGG "Use the built-in libogg library" ON)
option(BUILTIN_LIBPNG "Use the built-in libpng library" ON)
option(BUILTIN_LIBTHEORA "Use the built-in libtheora library" ON)
option(BUILTIN_LIBVORBIS "Use the built-in libvorbis library" ON)
option(BUILTIN_LIBWEBP "Use the built-in libwebp library" ON)
option(BUILTIN_WSLAY "Use the built-in wslay library" ON)
option(BUILTIN_MBEDTLS "Use the built-in mbedTLS library" ON)
option(BUILTIN_MINIUPNPC "Use the built-in miniupnpc library" ON)
option(BUILTIN_OPENXR "Use the built-in OpenXR library" ON)
option(BUILTIN_PCRE2 "Use the built-in PCRE2 library" ON)
option(BUILTIN_PCRE2_WITH_JIT "Use JIT compiler for the built-in PCRE2 library" ON)
option(BUILTIN_RECASTNAVIGATION "Use the built-in Recast navigation library" ON)
option(BUILTIN_RVO2_2D "Use the built-in RVO2 2D library" ON)
option(BUILTIN_RVO2_3D "Use the built-in RVO2 3D library" ON)
option(BUILTIN_XATLAS "Use the built-in xatlas library" ON)
option(BUILTIN_ZLIB "Use the built-in zlib library" ON)
option(BUILTIN_ZSTD "Use the built-in Zstd library" ON)

option(DISABLE_3D "Disable 3D nodes for a smaller executable" OFF)
option(DISABLE_ADVANCED_GUI "Disable advanced GUI nodes and behaviors" OFF)
option(DISABLE_PHYSICS_2D "Disable 2D physics nodes and server" OFF)
option(DISABLE_PHYSICS_3D "Disable 3D physics nodes and server" OFF)
option(DISABLE_NAVIGATION_2D "Disable 2D navigation features" OFF)
option(DISABLE_NAVIGATION_3D "Disable 3D navigation features" OFF)
option(DISABLE_XR "Disable XR nodes and server" OFF)
option(MODULES_ENABLED_BY_DEFAULT "If no disable all modules except ones explicitly enabled" ON)
option(NO_EDITOR_SPLASH "Don't use the custom splash screen for the editor" ON)
option(USE_PRECISE_MATH_CHECKS "Math checks use very precise epsilon (debug option)" OFF)
option(STRICT_CHECKS "Enforce stricter checks (debug option)" OFF)
option(SCU_BUILD "Use single compilation unit build" OFF)
option(ENGINE_UPDATE_CHECK "Enable engine update checks in the Project Manager" ON)
option(STEAMAPI "Enable minimal SteamAPI integration for usage time tracking (editor only)" OFF)


