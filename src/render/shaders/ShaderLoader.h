GLint *shader_files = new GLint[4];
GLint *shader_program = new GLint[2];

shader_files[0] = ShaderProgram::compileShader(0, "void main()\n{\n}\n"); // fontrender.c
shader_files[1] = ShaderProgram::compileShader(0, "int commonVar;\nint vertex_posititon;\nint testapple2;\n\n"); // test.c
shader_files[2] = ShaderProgram::compileShader(1, "int appleCField;\nint commonVar;\nint vertex_posititon;\nint testapple2;\n\n"); // apple.c
shader_files[3] = ShaderProgram::compileShader(0, ""); // smeg.c

shader_program[0] = new ShaderProgram("font", 0, 3, new GLint[]{shader_files[0], shader_files[1], shader_files[2]});
shader_program[1] = new ShaderProgram("test", 0, 2, new GLint[]{shader_files[1], shader_files[3]});

