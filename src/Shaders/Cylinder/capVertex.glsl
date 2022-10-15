#version 330

layout(location=0) in vec3 pos;

uniform mat4 projectionMatrix;
uniform mat4 rotationMatrix;
uniform float dist;

out vec2 vertexColour;

void main()
{
	gl_Position=projectionMatrix*(rotationMatrix*vec4(pos,1)+vec4(0,0,-dist,0));
}