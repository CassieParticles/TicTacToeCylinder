#version 330

in vec2 vertexTextCoords;

uniform sampler2D ourTexture;

layout(location=0)out vec3 FragColour;
layout(location=1)out vec3 FragCoord;

void main()
{
	FragColour=texture(ourTexture,vertexTextCoords).xyz;
	FragCoord=vec3(vertexTextCoords,1);
}