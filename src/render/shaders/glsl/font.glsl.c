#vertex
#version 150

import in vec3 vertexPosition;
import in vec2 vertexUV;

out vec2 uvCoord;

import uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vertexPosition, 1);
    uvCoord = vertexUV;
}

#fragment
#version 150

in vec2 uvCoord;
import uniform vec4 textColor;
import uniform sampler2D texture;

out vec4 color;

void main() {
	color = vec4(textColor.rgb, texture2D(texture, uvCoord).r * textColor.a);
}
