//fragment

in vec2 uvCoord;
import uniform vec4 textColor;
import(material_map_Kd) uniform sampler2D texture;

out vec4 color;

void main() {
	color = vec4(textColor.rgb, texture2D(texture, uvCoord).r * textColor.a);
}
