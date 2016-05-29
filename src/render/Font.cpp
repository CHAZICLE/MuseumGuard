#include <iostream>

#include "Font.hpp"
#include "render/shaders/ShaderUtils.hpp"

using namespace render;

FT_Library Font::library;
shaders::ShaderProgram *Font::shader = 0;
GLint Font::vertexPositionAttribute = 0;
GLint Font::vertexTextureAttribute = 0;
GLint Font::uniformTextColor = 0;
GLint Font::uniformTexture = 0;
Font::Font(std::string fontfamily, float heightMM)
{
	if(Font::library==0)
	{
		if(FT_Init_FreeType(&Font::library))
		{
			return;
		}
		Font::shader = shaders::ShaderProgram::getShader(SHADER_font);
		Font::vertexPositionAttribute = Font::shader->getShaderLocation(false, SHADERVAR_vertex_position);
		Font::vertexTextureAttribute = Font::shader->getShaderLocation(false, SHADERVAR_vertex_texture);
		Font::uniformTextColor = Font::shader->getShaderLocation(true, SHADER_font_textColor);
		Font::uniformTexture = Font::shader->getShaderLocation(true, SHADERVAR_material_map_Kd);
	}
	this->face = new FT_Face;
	FT_Byte fontData[] = {
		#include "cour.h"
	};
	if(FT_New_Memory_Face(Font::library, fontData, sizeof(fontData), 0, this->face))
	{
		std::cerr << "Failed to load font face" << std::endl;
		std::exit(1);
		return;
	}
	this->setHeight(heightMM);
	this->setColor(0.5f, 0.f, 1.0f, 1.f);
	glGenVertexArrays(1, &this->fontFaceVertexArrayObjectID);
	glGenTextures(128, this->fontFaceTextures);
	glGenBuffers(1, &this->fontFaceTextureCoordBufferID);
	glGenBuffers(1, &this->fontFaceTextureUVBufferID);
	for(int i=0;i<128;i++)
		this->metrics[i] = {0,0,0,0,0,0,0,false};
}
Font::~Font()
{
	FT_Done_FreeType(Font::library);
}
float Font::getTextWidth(std::string text, render::RenderManager &rManager)
{
	float calculatedPixelSize = this->heightMM*rManager.getHeightPx()/rManager.getHeightMM();
	float sx = rManager.getWidthMM()/rManager.getWidthPx();
	float totalWidth = 0;
	for(unsigned long i=0;i<text.length(); i++)
	{
		char c = text[i];
		struct GlyphMetrics *glyph = getGlyphMetrics_NoTexture(c, calculatedPixelSize);
		totalWidth += (glyph->width+(glyph->advanceX >> 6)) * sx;
	}
	return totalWidth;
}
void Font::printf(std::string text, render::RenderManager &rManager)
{
	// Prime Shader Program
	rManager.useShader(SHADER_font);
	glUniform4f(Font::uniformTextColor, this->r, this->g, this->b, this->a);
	glUniform1i(Font::uniformTexture, 0);

	// Set up vertex array/buffer objects
	glBindVertexArray(this->fontFaceVertexArrayObjectID);
	
	float calculatedPixelSize = this->heightMM*rManager.getHeightPx()/rManager.getHeightMM();
	float x=0,y=0,vx=1,vy=1,vw=10,vh=10,sx=rManager.getWidthMM()/rManager.getWidthPx(),sy=rManager.getHeightMM()/rManager.getHeightPx();
	rManager.enableTransparency();
	glActiveTexture(GL_TEXTURE0);
	for(unsigned long i=0;i<text.length(); i++)
	{
		char c = text[i];
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->fontFaceTextures[(int)c]);
		// (Re)Load the texture
		struct GlyphMetrics *glyph = getGlyphMetrics(c, calculatedPixelSize);
		
		// Get the coords
		vx = x + glyph->left * sx;
		vy = y + glyph->top * sy;
		vw = glyph->width * sx;
		vh = glyph->height * sy;
		GLfloat coords[] = {
			vx,		vy,
			vx,		vy - vh,
			vx + vw,	vy,
			vx + vw,	vy - vh,
		};
		GLuint uvCoords[] = {
			0,0,
			0,1,
			1,0,
			1,1,
		};
		
		glEnableVertexAttribArray(vertexPositionAttribute);
		glBindBuffer(GL_ARRAY_BUFFER, this->fontFaceTextureCoordBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(vertexPositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
		
		glEnableVertexAttribArray(vertexTextureAttribute);
		glBindBuffer(GL_ARRAY_BUFFER, this->fontFaceTextureUVBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uvCoords), uvCoords, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(vertexTextureAttribute, 2, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		glDisableVertexAttribArray(vertexPositionAttribute);
		glDisableVertexAttribArray(vertexTextureAttribute);
		
		x += (glyph->advanceX >> 6) * sx;
		y += (glyph->advanceY >> 6) * sy;
	}
	rManager.disableTransparency();
}
void Font::setHeight(float heightMM)
{
	this->heightMM = heightMM;
}
void Font::setColor(float r, float g, float b, float a)
{
	this->r = r;
	this->b = b;
	this->g = g;
	this->a = a;
}
struct GlyphMetrics *Font::getGlyphMetrics(char c, int calculatedPixelSize)
{
	struct GlyphMetrics *glyph = &this->metrics[(int)c];
	// Ensure glyph is up to date
	if(glyph->currentPixelSize==calculatedPixelSize)
		return glyph;
	glyph->currentPixelSize = calculatedPixelSize;
	// Update face pixel sizes
	if(FT_Set_Pixel_Sizes(*this->face, 0, calculatedPixelSize))
	{
		std::cerr << "Failed to set pixel sizes for font" << std::endl;
		std::exit(1);
	}
	if(FT_Load_Char(*this->face, c, FT_LOAD_RENDER))
	{
		std::cerr << "Failed to load character" << c << std::endl;
		std::exit(1);
	}
	FT_GlyphSlot g = (*this->face)->glyph;
	// Load the texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
	// Update the metrics
	glyph->top = g->bitmap_top;
	glyph->left = g->bitmap_left;
	glyph->width = g->bitmap.width;
	glyph->height = g->bitmap.rows;
	glyph->advanceX = g->advance.x;
	glyph->advanceY = g->advance.y;
	return metrics;
}
struct GlyphMetrics *Font::getGlyphMetrics_NoTexture(char c, int calculatedPixelSize)
{
	struct GlyphMetrics *glyph = &this->metrics[(int)c];
	// Ensure glyph is up to date
	if(glyph->currentPixelSize==calculatedPixelSize)
		return glyph;
	// Update face pixel sizes
	if(FT_Set_Pixel_Sizes(*this->face, 0, calculatedPixelSize))
	{
		std::cerr << "Failed to set pixel sizes for font" << std::endl;
		std::exit(1);
	}
	if(FT_Load_Char(*this->face, c, FT_LOAD_RENDER))
	{
		std::cerr << "Failed to load character" << c << std::endl;
		std::exit(1);
	}
	FT_GlyphSlot g = (*this->face)->glyph;
	// Update the metrics
	glyph->top = g->bitmap_top;
	glyph->left = g->bitmap_left;
	glyph->width = g->bitmap.width;
	glyph->height = g->bitmap.rows;
	glyph->advanceX = g->advance.x;
	glyph->advanceY = g->advance.y;
	return metrics;
}
