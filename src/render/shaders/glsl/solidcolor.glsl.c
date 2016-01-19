#vertex
#version 150

import uniform mat4 MVP;
import in vec3 vertexPosition;

void main() {
    gl_Position = MVP * vec4(vertexPosition, 1);
}

#fragment
#version 150

out vec4 color;
import uniform vec4 inColor;

void main() {
	color = inColor;
}
