//vertex

import in vec3 vertex_position;
import in vec3 vertex_color;

import uniform mat4 matrix_MVP;

out vec3 vertexColor;

void main() {
    gl_Position = matrix_MVP * vec4(vertex_position, 1);
    vertexColor = vertex_color;
}
