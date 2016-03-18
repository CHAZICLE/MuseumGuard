#include <iostream>

#include "Font.hpp"

FT_Library Font::library;
GLuint Font::textColorID = 0;
GLuint Font::textureID = 0;
Font::Font(const char *fontfilename, int heightMM)
{
	if(library==0 && FT_Init_FreeType(&library))
	{
		return;
	}
	if(FT_New_Face(Font::library, fontfilename, 0, &this->face))
	{
		std::cerr << "Load font face" << std::endl;
		return;
	}
	this->size_point = heightMM;
	glGenVertexArrays(1, &this->vertexArrayObjectID);
	glGenBuffers(2, &this->bufferObjectID[0]);
	glGenTextures(128, this->textures);
}
Font::~Font()
{
	FT_Done_FreeType(Font::library);
}
void Font::onResize(struct DisplayMetrics &metrics)
{
	if(FT_Set_Pixel_Sizes(this->face, 0, this->size_point*metrics.heightPx/metrics.heightMM))
	{
		std::cerr << "Load font face" << std::endl;
		return;
	}
	int error;
	FT_GlyphSlot g = this->face->glyph;
	glActiveTexture(GL_TEXTURE0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for(int c=0;c<128;c++)
	{
		error = FT_Load_Char(this->face, c, FT_LOAD_RENDER);
		if(error)
			continue;
		glBindTexture(GL_TEXTURE_2D, textures[c]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		struct GlyphMetrics *metrics = &this->metrics[c];
		metrics->top = g->bitmap_top;
		metrics->left = g->bitmap_left;
		metrics->width = g->bitmap.width;
		metrics->height = g->bitmap.rows;
		metrics->advanceX = g->advance.x;
		metrics->advanceY = g->advance.y;
	}
}
void Font::printf(const char *text, struct DisplayMetrics &metrics, glm::mat4 pMatrix)
{
	// Prime Shader Program
	glUseProgram(shaders::program_font);
	glUniformMatrix4fv(shaders::program_font_MVP, 1, GL_FALSE, &pMatrix[0][0]);
	glUniform4f(shaders::program_font_textColor, 0.5f, 0.f, 1.0f, 1.f);
	glUniform1i(shaders::program_font_texture, 0);
	
	// Set up vertex array/buffer objects
	glBindVertexArray(this->vertexArrayObjectID);
	
	const char *p;
	float x=0,y=0,vx=1,vy=1,vw=10,vh=10,sx=metrics.widthMM/metrics.widthPx,sy=metrics.widthMM/metrics.widthPx;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for(p = text; *p; p++)
	{
		struct GlyphMetrics *gi = &this->metrics[(int)*p];
		// Get the coords
		
		vx = x + gi->left * sx;
		vy = y + gi->top * sy;
		vw = gi->width * sx;
		vh = gi->height * sy;
		GLfloat coords[] = {
			vx,		vy,		0,
			vx,		vy - vh,	0,
			vx + vw,	vy,		0,
			vx + vw,	vy - vh,	0,
		};
		GLuint uvCoords[] = {
			0,0,
			0,1,
			1,0,
			1,1,
		};
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[(int)*p]);
		
		glEnableVertexAttribArray(shaders::program_font_vertexPosition);
		glBindBuffer(GL_ARRAY_BUFFER, this->bufferObjectID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(shaders::program_font_vertexPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		glEnableVertexAttribArray(shaders::program_font_vertexUV);
		glBindBuffer(GL_ARRAY_BUFFER, this->bufferObjectID[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uvCoords), uvCoords, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(shaders::program_font_vertexUV, 2, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		glDisableVertexAttribArray(shaders::program_font_vertexPosition);
		glDisableVertexAttribArray(shaders::program_font_vertexUV);
		
		x += (gi->advanceX >> 6) * sx;
		y += (gi->advanceY >> 6) * sy;
	}
}
