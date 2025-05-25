if (REX_PLATFORM STREQUAL "linux")
	target_sources(rex_engine PRIVATE
		asound-so_wrap.c
	)
endif()

target_sources(rex_engine PRIVATE
	audio_driver_alsa.cpp
)
