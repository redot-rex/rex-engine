if (REX_PLATFORM STREQUAL "linux")
	target_sources(rex_drivers PRIVATE
		pulse-so_wrap.c
	)
endif()

target_sources(rex_drivers PRIVATE
	audio_driver_pulseaudio.cpp
)
