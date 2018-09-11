add_executable(VolumeRendering
	QVolumeRenderer.h
	QVolumeRenderer.cpp
	dvr.cpp
)
target_link_libraries( VolumeRendering GetSet GetSetGui LibSimple)
qt5_use_modules(VolumeRendering Gui OpenGL)
install(TARGETS VolumeRendering DESTINATION bin)
