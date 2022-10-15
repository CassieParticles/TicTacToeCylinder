#version 330

layout(location=0) in vec2 position;
layout(location=1) in vec2 texCoord;

uniform vec2 translate;
uniform vec2 scale;

out vec2 fragTexCoord;

void main()
{
	gl_Position=vec4(translate+position*scale,0,1);
	fragTexCoord=texCoord;
}