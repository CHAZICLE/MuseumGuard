//fragment

in vec2 UV;
import uniform sampler2D material_map_Kd;

out vec4 color;

void main() {
	color = vec4(texture( material_map_Kd, UV ).rgb, 1.0f);
}
