#vertex
#version 150

import uniform mat4 MVP;

import in vec3 vertexPosition;
import in vec3 vertexColor;

out vec3 fragmentColor;

void main() {
    gl_Position = MVP * vec4(vertexPosition, 1);
    fragmentColor = vertexColor;
}

#fragment
#version 150

in vec3 fragmentColor;

out vec4 color;

void main() {
    color = vec4(fragmentColor, 1.0);
}
