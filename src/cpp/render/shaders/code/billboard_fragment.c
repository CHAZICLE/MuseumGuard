//fragment

in vec2 UV;
import uniform sampler2D material_map_Kd;

out vec4 color;

void main() {
	color = texture(material_map_Kd, UV).rgba;
}
