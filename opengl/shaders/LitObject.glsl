#version 330 core

#define PI 3.14156

out vec4 FragColor;
in vec3 outNormal;
in vec3 fragWorldPosition;
in vec2 outTexCoord;
in vec2 outTexCoord2;

struct DirectionalLightStruct{
	vec3 color;
	vec3 direction;
	float intensity;
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
	sampler2D emission;
	sampler2D reflection;
	sampler2D lightmap;
	float emissionStrength;
	float roughness;
	float metalic;
	float IOR;
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

/*
vec3 getAmbient(MaterialStruct mat, vec3 lightColor){

	float ambientStrength = 0.2;
	vec3 ambient =  ambientStrength * texture(mat.color, outTexCoord).rgb * lightColor;
	return ambient;
}


vec3 getDiffuse( MaterialStruct mat, vec3 normal,vec3 lightDirection, vec3 lightColor){

	float diffuseAmount = max(dot(normalize(lightDirection), normalize(normal)),0.0);
	vec3 diffuse = diffuseAmount* lightColor* texture(mat.color, outTexCoord).rgb;
	return vec3(diffuseAmount);
}

vec3 getSpecular(MaterialStruct mat, vec3 cameraPos, vec3 fragPos,vec3 lightColor, vec3 lightDir, vec3 normal){
	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectedLightDir = reflect(-normalize(lightDir), normalize(normal));
	vec3 halfwayDir = normalize(viewDir + reflectedLightDir);

	float specularAmount = pow(max(dot(normal, halfwayDir),0.0), mat.shine);

	vec3 specular = specularAmount* texture(mat.specular, outTexCoord).r * texture(mat.color, outTexCoord).rgb * lightColor;
	return specular;
}
*/

vec3 getSkyboxReflection(MaterialStruct mat, vec3 cameraPos, vec3 fragPos, vec3 normal, samplerCube skybox, vec2 texCoords){
	vec3 viewDir = normalize( fragPos - cameraPos);
	vec3 reflectedViewDir = reflect(viewDir, normalize(normal));
	return texture(skybox, reflectedViewDir).rgb * mat.reflectionStrength; ;
}

vec3 bakedLight(MaterialStruct mat,vec2 texCoords, vec2 lightmapTexCoords){
	return (texture(mat.lightmap, lightmapTexCoords)* texture(mat.color,texCoords)).rgb;
}

/*
vec3 directionalLight(DirectionalLightStruct light, MaterialStruct mat){
	
	
	vec3 ambient = getAmbient(mat, light.color);
	vec3 diffuse = getDiffuse(mat, outNormal, light.direction, light.color);
	vec3 specular = getSpecular(mat, uCameraPos, fragWorldPosition, light.color, light.direction, outNormal);
	

	vec3 result = ambient + diffuse + specular;
	return diffuse;

};

vec3 pointLight(PointLightStruct light, MaterialStruct mat){
	

	vec3 ambient = getAmbient(mat, light.color);
	vec3 diffuse = getDiffuse(mat, outNormal, normalize(light.position - fragWorldPosition), light.color);
	vec3 specular = getSpecular(mat, uCameraPos, fragWorldPosition, light.color, normalize(light.position - fragWorldPosition), outNormal);

	float distance    = length(light.position - fragWorldPosition);
	float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));   

	vec3 result = (ambient + diffuse + specular) * attenuation;
	return result;

};

*/

vec3 getF0(float IOR, vec3 color, float metalic){
	
	vec3 F0 = vec3(pow((1.0 - IOR)/(1.0 + IOR), 2));
	F0 = mix(F0, color, metalic);
	return F0;
}

float getAlpha(float roughness){
	return roughness* roughness;
}

vec3 fresnelSchlick(vec3 N, vec3 V, vec3 F0)
{
	float NDotV = max(dot(N, V), 0.005);

    return F0 + (1.0 - F0) * pow(NDotV, 5.0);
}

vec3 lambert(vec3 color){
	return color/PI;
}

float ndGGXTut(float roughness, vec3 N, vec3 H){
	
	float a      = roughness*roughness;
	float a2     = a*a;
	float NdotH  = max(dot(N, H), 0.005);
	float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	
    return num / denom;
}

float ndGGXPer(float roughness, vec3 N, vec3 H){
	
	float alpha = getAlpha(roughness);

	float NDotH = max(dot(N,H),0.005);

	float num = alpha * alpha;
	float denum = PI * pow(((NDotH*NDotH) * (alpha*alpha - 1) + 1),2);

	return num / denum;

}

float ndGGX(float roughness, vec3 N, vec3 H){
	return ndGGXPer(roughness,N,H);
}

float geometrySB(float k, vec3 N, vec3 X){

	float NDotX = max(dot(N,X),0.005);

	float num = NDotX;
	float denum = NDotX * (1 - k) + k;

	return num/denum;
}

float geometrySmith(float roughness, vec3 L, vec3 N, vec3 V){
	
	
	float k = getAlpha(roughness) / 2.0;

	float G1 = geometrySB(k, N, L);
	float G2 = geometrySB(k, N, V);

	return G1 * G2;
}

vec3 specularCookTorrance(float metalic, vec3 color,float roughness, float IOR, vec3 N, vec3 H, vec3 L, vec3 V){
	
	float VDotN = max(dot(V, N),0.005);
	float LDotN = max(dot(L, N),0.005);

	vec3 num =  fresnelSchlick( N, V, getF0(IOR, color, metalic)) * ndGGX( roughness, N, H) * geometrySmith(roughness, L, N, V) ;
	float denum = 4 * VDotN * LDotN + 0.001;
	return num / denum;

}

vec3 BRDF(float metalic, vec3 color, float roughness, float IOR, vec3 N, vec3 H, vec3 L, vec3 V){

	

	//vec3 albedo = mix(lambert(color), vec3(0.0), metalic);

	vec3 Kd = ( vec3(1.0) - fresnelSchlick( N, V, getF0(IOR, color, metalic) )) * (1.0 - metalic);
	float Ks = 1.0;

	vec3 diffuse = lambert(color) * Kd;
	vec3 specular = specularCookTorrance(metalic, color, roughness,IOR,N,H,L,V)* Ks;

	return diffuse + specular;
	//return Kd * lambert(color) + specularColor * specularCookTorrance(roughness,IOR,N,H,L,V)
	//return specularColor;
}

vec3 PBR (float metalic, vec3 emission, vec3 inLight, vec3 color, float roughness, float IOR, vec3 N, vec3 H, vec3 L, vec3 V){
	float LDotN = max(dot(normalize(L),normalize(N)),0.005);
	return emission + BRDF(metalic,color, roughness,IOR,N,H,L,V) * inLight * LDotN;
	//return vec3(LDotN,LDotN,LDotN);
}
void main()
{
	
	//vec3 dirLightAmount = directionalLight(Light,Mat);
	//vec3 pointLightAmount = pointLight(PointLight, Mat);
	//vec3 emissionAmount = getEmission(Mat);
	//vec3 skyboxReflectAmount = getSkyboxReflection(Mat, uCameraPos, fragWorldPosition, outNormal, skybox, outTexCoord);
	//vec3 bakedLightAmount = bakedLight(Mat,outTexCoord,outTexCoord2);

	vec3 V = normalize(uCameraPos - fragWorldPosition);
	vec3 L = normalize(Light.direction);
	vec3 H = normalize(V + L);
	vec3 N = normalize(outNormal);
	
	vec3 emission = texture(Mat.emission,outTexCoord).rgb * Mat.emissionStrength;
	float roughness = Mat.roughness;
	float IOR = Mat.IOR;
	vec3 color = texture(Mat.color, outTexCoord).rgb;
	float metalic = Mat.metalic;
	vec3 inLight = Light.color * Light.intensity;

	vec3 pbr = PBR(metalic, emission, inLight, color, roughness,IOR,N,H,L,V);
	FragColor = vec4(pbr, 1.0);	

//	vec4 result = vec4( pointLightAmount + emissionAmount +dirLightAmount + skyboxReflectAmount + bakedLightAmount, 1.0);


	

}