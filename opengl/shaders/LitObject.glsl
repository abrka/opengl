#version 330 core

out vec4 FragColor;
in vec3 outNormal;
in vec3 fragWorldPosition;
in vec2 outTexCoord;

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
	sampler2D color;
	sampler2D specular;
	sampler2D emission;
	float emissionStrength;
	float shine;
};

uniform DirectionalLightStruct Light;
uniform PointLightStruct PointLight;
uniform MaterialStruct Mat;
uniform vec3 uCameraPos;

uniform sampler3D skybox;


vec3 getEmission(MaterialStruct mat){
	return texture(mat.emission, outTexCoord).rgb * mat.emissionStrength;
}
vec3 getAmbient(MaterialStruct mat, vec3 lightColor){

	float ambientStrength = 0.2;
	vec3 ambient =  ambientStrength * texture(mat.color, outTexCoord).rgb * lightColor;
	return ambient;
}

vec3 getDiffuse( MaterialStruct mat, vec3 normal,vec3 lightDirection, vec3 lightColor){

	float diffuseAmount = max(dot(normalize(lightDirection), normal),0.0);
	vec3 diffuse = diffuseAmount* lightColor* texture(mat.color, outTexCoord).rgb;
	return diffuse;
}

vec3 getSpecular(MaterialStruct mat, vec3 cameraPos, vec3 fragPos,vec3 lightColor, vec3 lightDir){
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectedLightDir = reflect(-normalize(lightDir), outNormal);
	float specularAmount = pow(max(dot(viewDir, reflectedLightDir),0.0), mat.shine);
	vec3 specular = specularAmount* texture(mat.specular, outTexCoord).r * texture(mat.color, outTexCoord).rgb * lightColor;
	return specular;
}


vec3 directionalLight(DirectionalLightStruct light, MaterialStruct mat){
	
	
	vec3 ambient = getAmbient(mat, light.color);
	vec3 diffuse = getDiffuse(mat, outNormal, light.direction, light.color);
	vec3 specular = getSpecular(mat, uCameraPos, fragWorldPosition, light.color, light.direction);
	

	vec3 result = ambient + diffuse + specular;
	return result;

};

vec3 pointLight(PointLightStruct light, MaterialStruct mat){
	

	vec3 ambient = getAmbient(mat, light.color);
	vec3 diffuse = getDiffuse(mat, outNormal, normalize(light.position - fragWorldPosition), light.color);
	vec3 specular = getSpecular(mat, uCameraPos, fragWorldPosition, light.color, normalize(light.position - fragWorldPosition));

	/*float ambientStrength = 0.2;
	vec3 ambient =  ambientStrength *mat.ambient* light.color;

	vec3 lightDir = normalize(light.position - fragWorldPosition);
	
	float diffuseAmount = max(dot(lightDir, outNormal),0.0);
	vec3 diffuse = diffuseAmount* light.color* mat.color;

	vec3 viewDir = normalize(uCameraPos - fragWorldPosition);
	vec3 reflectedLightDir = reflect(-lightDir, outNormal);
	float specularExp = 32;
	float specularAmount = pow(max(dot(viewDir, reflectedLightDir),0.0), specularExp);
	vec3 specular = specularAmount* mat.specular * mat.color * light.color;*/

	float distance    = length(light.position - fragWorldPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));   

	vec3 result = (ambient + diffuse + specular) * attenuation;
	return result;

};

void main()
{
	
	FragColor = vec4(directionalLight(Light,Mat) + pointLight(PointLight, Mat) + getEmission(Mat), 1.0);
	
}