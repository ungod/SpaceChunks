#version 330 core

varying vec2 color;

uniform vec3 triangleColor;

out vec4 outColor;

void main()
{
	outColor = vec4(color, 1.0, 1.0);
}