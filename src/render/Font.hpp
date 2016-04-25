#ifndef __FONT_H_INCLUDED__
#define __FONT_H_INCLUDED__

namespace render {
	class Font;
}

#include <ft2build.h>
#include FT_FREETYPE_H
#include <cstdarg>
#include <string>
#include "util/gl.h"
#include "RenderManager.hpp"

#define FONT_RESOURCE_FOLDER "res/fonts/"

namespace render {
	struct GlyphMetrics {
		float currentPixelSize;
		int top,left,width,height,advanceX,advanceY;
		bool textureLoaded;
	};
	
	class Font {
		public:
			Font(std::string fontfamily, float heightMM);
			~Font();
			
			void printf(std::string text, render::RenderManager *rManager);
			float getTextWidth(std::string text, render::RenderManager *rManager);
			void setHeight(float heightMM);
			void setColor(float r, float g, float b, float a);
		private:
			static FT_Library library;
			FT_Face *face;
			struct GlyphMetrics metrics[128];
			float heightMM;
			int heightPX;
			GLuint fontFaceVertexArrayObjectID, fontFaceTextures[128], fontFaceTextureCoordBufferID, fontFaceTextureUVBufferID;
			float r,g,b,a;
			struct GlyphMetrics *getGlyphMetrics(char c, int calculatedPixelSize);
			struct GlyphMetrics *getGlyphMetrics_NoTexture(char c, int calculatedPixelSize);
	};
}

#endif
