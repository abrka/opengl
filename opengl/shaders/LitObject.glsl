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
	vec3 specular;
	vec3 ambient;
};

uniform LightStruct Light;
uniform MaterialStruct Mat;
uniform vec3 uCameraPos;

void main()
{
	
	float ambientStrength = 0.2;
	vec3 ambient =  ambientStrength *Mat.ambient* Light.color;

	vec3 lightDir = normalize(Light.position- fragWorldPosition );
	float diffuseAmount = max(dot(lightDir, outNormal),0.0);
	vec3 diffuse = diffuseAmount* Light.color* Mat.color;

	vec3 viewDir = normalize(uCameraPos - fragWorldPosition);
	vec3 reflectedLightDir = reflect(-lightDir, outNormal);
	float specularExp = 16;
	float specularAmount = pow(max(dot(viewDir, reflectedLightDir),0.0), specularExp);
	vec3 specular = specularAmount* Mat.specular * Mat.color * Light.color;

	vec3 result = (ambient + diffuse + specular);
	
	FragColor = vec4(result, 1.0);
}