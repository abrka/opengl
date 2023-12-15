#version 330 core

out vec4 FragColor;
in vec3 outNormal;
in vec3 fragWorldPosition;

struct LightStruct{
	vec3 color;
	vec3 position;
};

struct MaterialStruct{
	vec3 color;
};

uniform LightStruct Light;
uniform MaterialStruct Mat;


void main()
{
	
	float ambientStrength = 0.4;
	vec3 ambient = Light.color* ambientStrength * Mat.color;

	vec3 lightDir = normalize(Light.position- fragWorldPosition );
	vec3 diffuse = dot(lightDir, outNormal) * Mat.color * Light.color;
	vec3 result = diffuse;
	
	FragColor = vec4(result, 1.0);
}