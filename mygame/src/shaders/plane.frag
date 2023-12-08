//FRAGMENT SHADER
#version 330 core
uniform float offset;

out vec4 FragColor;

void main()
{
	FragColor = vec4(tan(time), cos(time), sin(time), 1.0);
}
