//vertex

import in vec3 vertex_position;
import in vec3 vertex_color;

import uniform mat4 matrix_M;
import uniform mat4 matrix_V;
import uniform mat4 matrix_P;

out vec3 vertexColor;

void main() {
    gl_Position = matrix_P * matrix_V * matrix_M * vec4(vertex_position, 1);
    vertexColor = vertex_color;
}
