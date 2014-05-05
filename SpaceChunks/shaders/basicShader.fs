#version 400 core

uniform vec4 inputColour;

out vec4 fragColour;

void main()
{
	fragColour = inputColour;
}