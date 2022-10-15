#version 330

in vec2 fragTexCoords;

uniform sampler2D tex;

layout(location=0) out vec4 fragColour;

void main()
{
	fragColour=vec4(texture(tex,fragTexCoords).xyz,0.6);
}