#version 330 core

out vec4 FragColor;
in vec3 outNormal;
in vec3 fragWorldPosition;

struct DirectionalLightStruct{
	vec3 color;
	vec3 direction;
};

struct PointLightStruct{
	vec3 color;
	vec3 position;
	float constant;
    float linear;
    float quadratic;
};

struct MaterialStruct{
	vec3 color;
	vec3 specular;
	vec3 ambient;
};

uniform DirectionalLightStruct Light;
uniform PointLightStruct PointLight;
uniform MaterialStruct Mat;
uniform vec3 uCameraPos;

vec3 directionalLight(DirectionalLightStruct light, MaterialStruct mat){
	
	float ambientStrength = 0.2;
	vec3 ambient =  ambientStrength *mat.ambient* light.color;

	float diffuseAmount = max(dot(normalize(-light.direction), outNormal),0.0);
	vec3 diffuse = diffuseAmount* light.color* mat.color;

	vec3 viewDir = normalize(uCameraPos - fragWorldPosition);
	vec3 reflectedLightDir = reflect(normalize(light.direction), outNormal);
	float specularExp = 32;
	float specularAmount = pow(max(dot(viewDir, reflectedLightDir),0.0), specularExp);
	vec3 specular = specularAmount* mat.specular * mat.color * light.color;

	vec3 result = ambient + diffuse + specular;
	return result;

};

vec3 pointLight(PointLightStruct light, MaterialStruct mat){
	
	float ambientStrength = 0.2;
	vec3 ambient =  ambientStrength *mat.ambient* light.color;

	vec3 lightDir = normalize(light.position - fragWorldPosition);
	
	float diffuseAmount = max(dot(lightDir, outNormal),0.0);
	vec3 diffuse = diffuseAmount* light.color* mat.color;

	vec3 viewDir = normalize(uCameraPos - fragWorldPosition);
	vec3 reflectedLightDir = reflect(-lightDir, outNormal);
	float specularExp = 32;
	float specularAmount = pow(max(dot(viewDir, reflectedLightDir),0.0), specularExp);
	vec3 specular = specularAmount* mat.specular * mat.color * light.color;

	float distance    = length(light.position - fragWorldPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));   

	vec3 result = (ambient + diffuse + specular) * attenuation;
	return result;

};

void main()
{
	
	FragColor = vec4(directionalLight(Light, Mat) + pointLight(PointLight, Mat), 1.0);
}