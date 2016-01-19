/*
 * THIS FILE IS AUTO GENERATED
 * DO NOT EDIT!
 */
void compileShaders() {
	const char *vertex_code;
	const char *fragment_code;

	vertex_code="#version 150\n\nin vec3 vertexPosition;\nin vec2 vertexUV;\n\nout vec2 uvCoord;\n\nuniform mat4 MVP;\n\nvoid main() {\ngl_Position = MVP * vec4(vertexPosition, 1);\nuvCoord = vertexUV;\n}\n\n";
	fragment_code="#version 150\n\nin vec2 uvCoord;\nuniform vec4 textColor;\nuniform sampler2D texture;\n\nout vec4 color;\n\nvoid main() {\ncolor = vec4(textColor.rgb, texture2D(texture, uvCoord).r * textColor.a);\n}\n";
	program_font = loadShaders("font", vertex_code, fragment_code);
		program_font_vertexPosition = getAttributeLocation(program_font, "vertexPosition");
		program_font_vertexUV = getAttributeLocation(program_font, "vertexUV");
		program_font_MVP = getUniformLocation(program_font, "MVP");
		program_font_textColor = getUniformLocation(program_font, "textColor");
		program_font_texture = getUniformLocation(program_font, "texture");

	vertex_code="#version 150\n\nin vec3 vertexPosition;\n\nuniform mat4 MVP;\n\nout vec3 posOut;\n\nvoid main() {\ngl_Position = MVP * vec4(vertexPosition, 1);\nposOut = vertexPosition;\n}\n\n";
	fragment_code="#version 150\n\nout vec4 color;\nin vec3 posOut;\n\nvoid main() {\ncolor = vec4(posOut, 0.8);\n}\n";
	program_modelTest = loadShaders("modelTest", vertex_code, fragment_code);
		program_modelTest_vertexPosition = getAttributeLocation(program_modelTest, "vertexPosition");
		program_modelTest_MVP = getUniformLocation(program_modelTest, "MVP");

	vertex_code="#version 150\n\nin vec3 vertexPosition;\nin vec2 vertexUV;\n\nout vec2 UV;\n\nuniform mat4 MVP;\n\nvoid main() {\ngl_Position = MVP * vec4(vertexPosition, 1);\nUV = vertexUV;\n}\n\n";
	fragment_code="#version 150\n\nin vec2 UV;\nuniform sampler2D myTextureSampler;\n\nout vec3 color;\n\nvoid main() {\ncolor = texture( myTextureSampler, UV ).rgb;\n//color = texture(textureSampler, uvCoord).rgb;\n//color = vec4(vec3(uvCoord, 0.0f), 1.0f);\n}\n";
	program_modelTexture = loadShaders("modelTexture", vertex_code, fragment_code);
		program_modelTexture_vertexPosition = getAttributeLocation(program_modelTexture, "vertexPosition");
		program_modelTexture_vertexUV = getAttributeLocation(program_modelTexture, "vertexUV");
		program_modelTexture_MVP = getUniformLocation(program_modelTexture, "MVP");
		program_modelTexture_myTextureSampler = getUniformLocation(program_modelTexture, "myTextureSampler");

	vertex_code="#version 150\n\nuniform mat4 MVP;\nin vec3 vertexPosition;\n\nvoid main() {\ngl_Position = MVP * vec4(vertexPosition, 1);\n}\n\n";
	fragment_code="#version 150\n\nout vec4 color;\nuniform vec4 inColor;\n\nvoid main() {\ncolor = inColor;\n}\n";
	program_solidcolor = loadShaders("solidcolor", vertex_code, fragment_code);
		program_solidcolor_MVP = getUniformLocation(program_solidcolor, "MVP");
		program_solidcolor_vertexPosition = getAttributeLocation(program_solidcolor, "vertexPosition");
		program_solidcolor_inColor = getUniformLocation(program_solidcolor, "inColor");

	vertex_code="#version 150\n\nuniform mat4 MVP;\n\nin vec3 vertexPosition;\nin vec3 vertexColor;\n\nout vec3 fragmentColor;\n\nvoid main() {\ngl_Position = MVP * vec4(vertexPosition, 1);\nfragmentColor = vertexColor;\n}\n\n";
	fragment_code="#version 150\n\nin vec3 fragmentColor;\n\nout vec4 color;\n\nvoid main() {\ncolor = vec4(fragmentColor, 1.0);\n}\n";
	program_vertexcolor = loadShaders("vertexcolor", vertex_code, fragment_code);
		program_vertexcolor_MVP = getUniformLocation(program_vertexcolor, "MVP");
		program_vertexcolor_vertexPosition = getAttributeLocation(program_vertexcolor, "vertexPosition");
		program_vertexcolor_vertexColor = getAttributeLocation(program_vertexcolor, "vertexColor");

	}
	GLuint program_font = 0;
	GLint program_font_vertexPosition = 0;
	GLint program_font_vertexUV = 0;
	GLint program_font_MVP = 0;
	GLint program_font_textColor = 0;
	GLint program_font_texture = 0;
	GLuint program_modelTest = 0;
	GLint program_modelTest_vertexPosition = 0;
	GLint program_modelTest_MVP = 0;
	GLuint program_modelTexture = 0;
	GLint program_modelTexture_vertexPosition = 0;
	GLint program_modelTexture_vertexUV = 0;
	GLint program_modelTexture_MVP = 0;
	GLint program_modelTexture_myTextureSampler = 0;
	GLuint program_solidcolor = 0;
	GLint program_solidcolor_MVP = 0;
	GLint program_solidcolor_vertexPosition = 0;
	GLint program_solidcolor_inColor = 0;
	GLuint program_vertexcolor = 0;
	GLint program_vertexcolor_MVP = 0;
	GLint program_vertexcolor_vertexPosition = 0;
	GLint program_vertexcolor_vertexColor = 0;

