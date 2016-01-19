#vertex
#version 150

import in vec3 vertexPosition;
import in vec2 vertexUV;

out vec2 UV;

import uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(vertexPosition, 1);
    UV = vertexUV;
}

#fragment
#version 150

in vec2 UV;
import uniform sampler2D myTextureSampler;

out vec3 color;

void main() {
	color = texture( myTextureSampler, UV ).rgb;
	//color = texture(textureSampler, uvCoord).rgb;
	//color = vec4(vec3(uvCoord, 0.0f), 1.0f);
}
