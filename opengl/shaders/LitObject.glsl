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
	sampler2D reflection;
	float emissionStrength;
	float shine;
	float reflectionStrength;
};

uniform DirectionalLightStruct Light;
uniform PointLightStruct PointLight;
uniform MaterialStruct Mat;
uniform vec3 uCameraPos;

uniform samplerCube skybox;


vec3 getEmission(MaterialStruct mat){
	return texture(mat.emission, outTexCoord).rgb * mat.emissionStrength;
}
vec3 getAmbient(MaterialStruct mat, vec3 lightColor){

	float ambientStrength = 0.2;
	vec3 ambient =  ambientStrength * texture(mat.color, outTexCoord).rgb * lightColor;
	return ambient;
}

vec3 getDiffuse( MaterialStruct mat, vec3 normal,vec3 lightDirection, vec3 lightColor){

	float diffuseAmount = max(dot(normalize(lightDirection), normalize(normal)),0.0);
	vec3 diffuse = diffuseAmount* lightColor* texture(mat.color, outTexCoord).rgb;
	return diffuse;
}

vec3 getSpecular(MaterialStruct mat, vec3 cameraPos, vec3 fragPos,vec3 lightColor, vec3 lightDir, vec3 normal){
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectedLightDir = reflect(-normalize(lightDir), normalize(normal));
	vec3 halfwayDir = normalize(viewDir + reflectedLightDir);

	float specularAmount = pow(max(dot(normal, halfwayDir),0.0), mat.shine);

	vec3 specular = specularAmount* texture(mat.specular, outTexCoord).r * texture(mat.color, outTexCoord).rgb * lightColor;
	return specular;
}

vec3 getSkyboxReflection(MaterialStruct mat, vec3 cameraPos, vec3 fragPos, vec3 normal, samplerCube skybox, vec2 texCoords){
	vec3 viewDir = normalize( fragPos - cameraPos);
	vec3 reflectedViewDir = reflect(viewDir, normalize(normal));
	return texture(skybox, reflectedViewDir).rgb * mat.reflectionStrength; ;
}


vec3 directionalLight(DirectionalLightStruct light, MaterialStruct mat){
	
	
	vec3 ambient = getAmbient(mat, light.color);
	vec3 diffuse = getDiffuse(mat, outNormal, light.direction, light.color);
	vec3 specular = getSpecular(mat, uCameraPos, fragWorldPosition, light.color, light.direction, outNormal);
	

	vec3 result = ambient + diffuse + specular;
	return result;

};

vec3 pointLight(PointLightStruct light, MaterialStruct mat){
	

	vec3 ambient = getAmbient(mat, light.color);
	vec3 diffuse = getDiffuse(mat, outNormal, normalize(light.position - fragWorldPosition), light.color);
	vec3 specular = getSpecular(mat, uCameraPos, fragWorldPosition, light.color, normalize(light.position - fragWorldPosition), outNormal);

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
	
	vec3 dirLightAmount = directionalLight(Light,Mat);
	vec3 pointLightAmount = pointLight(PointLight, Mat);
	vec3 emissionAmount = getEmission(Mat);
	vec3 skyboxReflectAmount = getSkyboxReflection(Mat, uCameraPos, fragWorldPosition, outNormal, skybox, outTexCoord);
	vec4 result = vec4( pointLightAmount + emissionAmount +dirLightAmount + skyboxReflectAmount, 1.0);
	FragColor = result;

	

}