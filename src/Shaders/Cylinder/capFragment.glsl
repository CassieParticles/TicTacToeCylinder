#version 330

uniform vec3 colour;

layout(location=0)out vec3 FragColour;
layout(location=1)out vec3 FragCoord;	//Will always be 0, rendering to this to prevent it being "transparent" to the texture sampler

void main()
{
	FragColour=colour;
	FragCoord=vec3(0,0,0);
}