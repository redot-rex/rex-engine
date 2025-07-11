if (REX_PLATFORM STREQUAL "linuxbsd")
	if (USE_SOWRAP)
		target_sources(rex_drivers PRIVATE
			asound-so_wrap.c
		)
	endif()
endif()

target_sources(rex_drivers PRIVATE
	audio_driver_alsa.cpp
)
