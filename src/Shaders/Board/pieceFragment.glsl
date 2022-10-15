#version 330

in vec2 fragTexCoord;

uniform sampler2D tex;

layout(location=0)out vec4 fragColour;

void main()
{
	fragColour=texture(tex,fragTexCoord);
}