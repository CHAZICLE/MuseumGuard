shader_files = new GLint[13];
shader_program = (ShaderProgram **)malloc(sizeof(ShaderProgram *)*4);

shader_files[0] = ShaderProgram::compileShader("fontvertex.c", 0, "#version 330\nin vec3 vertex_position;\nin vec2 vertex_texture;\nout vec2 uvCoord;\nuniform mat4 matrix_MVP;\nvoid main() {\ngl_Position = matrix_MVP * vec4(vertex_position, 1);\nuvCoord = vertex_texture;\n}\n");
shader_files[1] = ShaderProgram::compileShader("fontfragment.c", 1, "#version 330\nin vec2 uvCoord;\nuniform vec4 textColor;\nuniform sampler2D texture;\nout vec4 color;\nvoid main() {\ncolor = vec4(textColor.rgb, texture2D(texture, uvCoord).r * textColor.a);\n}\n");
shader_files[2] = ShaderProgram::compileShader("fuzzymodel_vertex.c", 0, "#version 330\nin vec3 vertex_position;\nin vec3 vertex_color;\nuniform mat4 matrix_MVP;\nout vec3 vertexColor;\nvoid main() {\ngl_Position = matrix_MVP * vec4(vertex_position, 1);\nvertexColor = vertex_color;\n}\n");
shader_files[3] = ShaderProgram::compileShader("fuzzymodel_fragment.c", 1, "#version 330\nin vec3 vertexColor;\nout vec4 color;\nvoid main() {\ncolor = vec4(vertexColor, 1.0f);\n}\n");
shader_files[4] = ShaderProgram::compileShader("modelTexture_vertex.c", 0, "#version 330\nin vec3 vertex_position;\nin vec2 vertex_texture;\nout vec2 UV;\nuniform mat4 matrix_MVP;\nvoid main() {\ngl_Position = matrix_MVP * vec4(vertex_position, 1);\nUV = vertex_texture;\n}\n");
shader_files[5] = ShaderProgram::compileShader("modelTexture_fragment.c", 1, "#version 330\nin vec2 UV;\nuniform sampler2D material_map_Kd;\nout vec4 color;\nvoid main() {\ncolor = vec4(texture( material_map_Kd, UV ).rgb, 1.0f);\n}\n");
shader_files[6] = ShaderProgram::compileShader("UVTest_vertex.c", 0, "#version 330\nin vec3 vertex_position;\nin vec2 vertex_texture;\nout vec2 UV;\nuniform mat4 matrix_MVP;\nvoid main() {\ngl_Position = matrix_MVP * vec4(vertex_position, 1);\nUV = vertex_texture;\n}\n");
shader_files[7] = ShaderProgram::compileShader("UVText_fragment.c", 1, "#version 330\nin vec2 UV;\nuniform sampler2D material_map_Kd;\nout vec4 color;\nvoid main() {\ncolor = vec4(texture( material_map_Kd, UV ).rgb, 1.0f);\n}\n");

shader_program[0] = new ShaderProgram(0, "font", {shader_files[0], shader_files[1]}, 11, 524288, {"texture"}, {"textColor"});
shader_program[1] = new ShaderProgram(1, "fuzzyModel", {shader_files[2], shader_files[3]}, 9, 0, {}, {"vertex_color"});
shader_program[2] = new ShaderProgram(2, "modelTexture", {shader_files[4], shader_files[5]}, 524299, 0, {}, {});
shader_program[3] = new ShaderProgram(3, "UVTest", {shader_files[6], shader_files[7]}, 524299, 0, {}, {});

