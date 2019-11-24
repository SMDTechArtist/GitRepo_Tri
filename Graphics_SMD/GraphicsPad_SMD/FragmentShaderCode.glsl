#version 430

out vec4 daColor;
in vec3 worldNormal;
in vec3 vertexWorldPosition;

uniform vec3 lightPosition;

void main()
{
	vec3 lightVector = normalize(lightPosition - vertexWorldPosition);
	float brightness = dot(lightVector, normalize(worldNormal));
	daColor = vec4(brightness, brightness, brightness, 1.0);
}
