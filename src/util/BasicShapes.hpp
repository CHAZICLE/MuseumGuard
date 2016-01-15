#ifndef __BASICSHAPES_H_INCLUDED__
#define __BASICSHAPES_H_INCLUDED__

#include "util/gl.h"

class BasicShapes {
	private:
		static GLuint square_vertexArrayID,square_vertexBufferID;
	public:
		void preload();
		void renderSquare(float x, float y, float width, float height);
};

#endif
