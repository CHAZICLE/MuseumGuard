#ifndef __GL_H_INCLUDED__
#define __GL_H_INCLUDED__

#ifdef YCM
	#define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
	#include <GL/glext.h>
#else
	#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#endif
