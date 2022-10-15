#version 330

layout(location=0) in vec3 pos;
layout(location=1) in vec2 textCoords;

uniform mat4 projectionMatrix;
uniform mat4 rotationMatrix;
uniform float dist;

out vec2 vertexTextCoords;

void main()
{
	gl_Position=projectionMatrix*(rotationMatrix*vec4(pos,1)+vec4(0,0,-dist,0));
	vertexTextCoords=textCoords;
}