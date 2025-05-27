if (REX_PLATFORM STREQUAL "linux")
	target_sources(rex_drivers PRIVATE
		asound-so_wrap.c
	)
endif()

target_sources(rex_drivers PRIVATE
	audio_driver_alsa.cpp
)
