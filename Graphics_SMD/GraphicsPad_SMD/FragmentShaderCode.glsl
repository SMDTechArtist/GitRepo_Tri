#version 430

out vec4 daColor;

in vec3 theNormal;
in vec3 thePosition;

uniform vec3 lightPosition;
uniform vec4 ambientLight;
uniform vec3 cameraPosition;

//uniform vec3 dominatingColor;

void main()
{
	//Deffuse Light
	vec3 lightVector = normalize(lightPosition - thePosition);
	float brightness = dot(lightVector, normalize(theNormal));
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);

	//Specular Light
	vec3 reflected = reflect(-1 * lightVector, theNormal);
	vec3 camera = normalize(cameraPosition - thePosition);
	float s = clamp(dot(reflected, camera), 0, 1);
	s = pow(s, 60);

	vec4 spec = vec4(s, s, s, 1.0f);

	daColor = ambientLight + clamp(diffuseLight, 0, 1) + spec;


};

