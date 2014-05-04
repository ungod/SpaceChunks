#version 330 core

in vec2 position;

varying vec2 color;

void main()
{
	color = position;
	gl_Position = vec4(position, 0.0, 1.0);
}