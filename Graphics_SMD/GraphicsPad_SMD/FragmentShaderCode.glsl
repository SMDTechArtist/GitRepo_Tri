#version 430

out vec4 daColor;
in vec3 theColor;

//uniform vec3 dominatingColor;

void main()
{
	daColor = vec4(theColor, 1.0);

};

