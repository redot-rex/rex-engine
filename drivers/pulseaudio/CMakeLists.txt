if (REX_PLATFORM STREQUAL "linux")
	target_sources(rex_engine PRIVATE
		pulse-so_wrap.c
	)
endif()

target_sources(rex_engine PRIVATE
	audio_driver_pulseaudio.cpp
)
