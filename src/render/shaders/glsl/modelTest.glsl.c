#vertex
#version 150

import in vec3 vertexPosition;

import uniform mat4 MVP;

out vec3 posOut;

void main() {
    gl_Position = MVP * vec4(vertexPosition, 1);
    posOut = vertexPosition;
}

#fragment
#version 150

out vec4 color;
in vec3 posOut;

void main() {
	color = vec4(posOut, 0.8);
}
