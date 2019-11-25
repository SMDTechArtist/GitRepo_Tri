#version 430

out vec4 daColor;
in vec3 worldNormal;
in vec3 vertexWorldPosition;

uniform vec4 ambientLight;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform sampler2D Tex1;

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
	 s = pow(s, 60);

	 vec4 specularLight = clamp(vec4(s, s, s, 1), 0, 1);

	 //Texture
	 vec4 texColor = texture(Tex1, TexCoord);
	 vec4 ambAndDiff = clamp(diffuseLight, 0, 1) + ambientLight;

	 daColor = ambAndDiff * texColor + clamp(specularLight, 0, 1);
}
