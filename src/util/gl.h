#ifndef __GL_H_INCLUDED__
#define __GL_H_INCLUDED__

#ifdef USE_GLEW
	#include <GL/glew.h>
#else
	#define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
	#include <GL/glext.h>
#endif

#include <GLFW/glfw3.h>

#endif
