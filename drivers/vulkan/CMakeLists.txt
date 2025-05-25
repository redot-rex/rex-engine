file(GLOB_RECURSE VULKAN_SOURCES *.cpp)
target_sources(rex_engine PRIVATE ${VULKAN_SOURCES})
