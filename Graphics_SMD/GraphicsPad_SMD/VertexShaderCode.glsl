
#version 430

in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normalModel;
in layout(location=3) vec2 VertexTexCoord;



uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;

out vec3 worldNormal;
out vec3 vertexWorldPosition;
out vec2 texCoord;

void main()
{
	texCoord = VertexTexCoord;
	gl_Position = modelToProjectionMatrix * vertexPositionModel;
	worldNormal = vec3(modelToWorldMatrix * vec4(normalModel, 0.0));
	vertexWorldPosition = vec3(modelToWorldMatrix * vertexPositionModel);
}

