#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 Color;


void main()
{

	gl_Position = vec4(position, 0.0, 1.0);
	
};

