#version 430

in layout(location=0) vec2 triangle1Position;
in layout(location=1) vec3 triangle1Color;
in layout(location=2) vec2 triangle2Position;
in layout(location=1) vec3 triangle2Color;

out vec3 tri1Color;

void main()
{

	gl_Position = vec4(triangle1Position, 0.0, 1.0);
	gl_Position = vec4(triangle2Position, 0.0, 1.0);
};

