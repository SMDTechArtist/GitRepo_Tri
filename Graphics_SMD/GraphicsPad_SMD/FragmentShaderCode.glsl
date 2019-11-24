#version 430

out vec4 daColor;
in vec3 worldNormal;
in vec3 thePosition;

uniform vec3 lightPosition;

void main()
{
	vec3 lightVector = normalize(lightPosition - thePosition);
	float brightness = dot(lightVector, normalize(worldNormal));
	daColor = vec4(brightness, brightness, brightness, 1.0);
}
