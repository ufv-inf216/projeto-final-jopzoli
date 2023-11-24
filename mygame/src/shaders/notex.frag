#version 330 core

in vec3 colorPos;
out vec4 fragColor;

void main()
{
	fragColor.xyz = normalize(colorPos);
	fragColor.w = 1.f;
}
