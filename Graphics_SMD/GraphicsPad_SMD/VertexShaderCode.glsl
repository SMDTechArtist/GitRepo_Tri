#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;
in layout(location=2) mat4 fullTransformMatrix;
in layout(location=3) vec3 normal;

//uniform mat4 fullTransformMatrix;

out vec3 theNormal;
out vec3 thePosition;

void main()
{
	vec4 v = vec4(position, 1.0);
	gl_Position = fullTransformMatrix * v;
	theNormal = normal;
	thePosition = position;
};

