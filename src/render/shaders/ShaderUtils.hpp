#ifndef __SHADERUTILS_H_INCLUDED__
#define __SHADERUTILS_H_INCLUDED__

#include "util/gl.h"

namespace shaders {
	GLuint loadShaders(const char *programName, const char *vertexCode, const char *fragmentCode);
	GLint getUniformLocation(const GLuint programID, const char *uniformName);
	GLint getAttributeLocation(const GLuint programID, const char *attributeName);
#ifndef SKIP_DEPEND_TREE
#include "shadercode.h"
#endif
}

#endif
