#version 430

out vec3 daColor;

uniform vec3 dominatingColor;

void main()
{
	daColor = dominatingColor;
}

