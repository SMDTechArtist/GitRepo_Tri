
#version 430

in layout(location=0) vec3 vertexPositionModel;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normalModel;
uniform vec3 ambientLight;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;

out vec3 theNormal;
out vec3 thePosition;

void main()
{
	vec4 v = vec4(vertexPositionModel, 1.0);
	gl_Position = modelToProjectionMatrix * v;
	theNormal = vec3(modelToWorldMatrix * vec4(normalModel, 0.0));
	thePosition = vec3(modelToWorldMatrix * v);
}

