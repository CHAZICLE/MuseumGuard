//vertex

import in vec3 vertex_position;

import uniform vec3 billboard_center;
import uniform vec2 screen_scaler;

import uniform mat4 matrix_V;
import uniform mat4 matrix_P;

out vec2 UV;

void main() {
	gl_Position = matrix_P * matrix_V * vec4(billboard_center, 1.f);
	gl_Position /= gl_Position.w;
	gl_Position.xy += (vertex_position.xy*vec2(2.f, 2.f)-vec2(1.f, 1.f))*screen_scaler;
	UV = vertex_position.xy;
}
