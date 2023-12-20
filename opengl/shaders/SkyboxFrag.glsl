#version 330 core

out vec4 FragColor;

in vec3 texCoords;
uniform samplerCube skybox;

void main()
{
	FragColor = vec4(texture(skybox, texCoords).rgb,1.0);

	float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}