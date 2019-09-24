#version 430

in layout(location=0) vec3 position;


//uniform vec3 offset;


void main()
{
	gl_Position = vec4(position, 1.0);
	//gl_Position = vec4(position + offset, 1.0);
};

