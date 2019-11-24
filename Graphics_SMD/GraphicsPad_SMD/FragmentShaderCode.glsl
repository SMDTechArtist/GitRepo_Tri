#version 430

out vec4 daColor;
in vec3 worldNormal;
in vec3 vertexWorldPosition;

uniform vec4 ambientLight;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main()
{
	//Diffuse
	vec3 lightVector = normalize(lightPosition - vertexWorldPosition);
	float brightness = dot(lightVector, normalize(worldNormal));
	 vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);

	 //Specular
	 vec3 reflectedLightVectorWorld = reflect(-lightVector, worldNormal);
	 vec3 cameraVectorPosition = normalize(cameraPosition - vertexWorldPosition);
	 float s = dot(reflectedLightVectorWorld, cameraVectorPosition);
	 vec4 specularLight = vec4(s, s, s, 1);

	 daColor = clamp(diffuseLight, 0, 1) + ambientLight + clamp(specularLight, 0, 1);
}
