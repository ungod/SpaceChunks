#version 400 core

in vec3 position;

uniform mat4 matrix;

void main()
{
	gl_Position = vec4(position, 1.0);
}