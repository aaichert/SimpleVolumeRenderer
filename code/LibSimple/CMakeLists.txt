add_library(LibSimple STATIC
	GL/glad.c
	BufferUtils.h
	SimpleMHD.h
	SimpleMHD.cpp
	SimpleGL.h
	SimpleGL.cpp
	SimpleVR.h
	SimpleVR.cpp
	SimpleRayCastPass.h
	SimpleRayCastPass.cpp
)
qt5_use_modules(LibSimple Gui OpenGL)

install(FILES GL/glad.h DESTINATION include/LibSimple/GL )
install(FILES QVolumeRenderer.h SimpleMHD.h SimpleGL.h SimpleVR.h SimpleRayCastPass.h DESTINATION include/LibSimple)
install(TARGETS LibSimple DESTINATION lib EXPORT LibSimple-targets)
install(EXPORT LibSimple-targets DESTINATION cmake)
