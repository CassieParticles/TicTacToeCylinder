#version 330

layout(location=0) in vec2 position;
layout(location=1) in vec2 texCoords;

out vec2 fragTexCoords;

void main()
{
	gl_Position=vec4(position,0,1);
	fragTexCoords=texCoords;
}