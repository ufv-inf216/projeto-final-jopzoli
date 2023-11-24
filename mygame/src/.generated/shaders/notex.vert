R"//(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 colorPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    colorPos = aPos;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)//"