//fragment

in vec2 UV;
import uniform sampler2D material_map_Kd;

out vec4 color;

void main() {
	color = vec4(texture( material_map_Kd, UV ).rgb, 1.0f);
	//color = vec4(vec3(UV.rg, 0.0f), 1.0f);
}
