//vertex

import in vec3 vertex_position;
import in vec2 vertex_texture;

out vec2 uvCoord;

import uniform mat4 matrix_MVP;

void main() {
    gl_Position = matrix_MVP * vec4(vertex_position, 1);
    uvCoord = vertex_texture;
}
