#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
									
out vec3 outNormal;
out vec3 fragWorldPosition;
out vec2 outTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
										
	outNormal = mat3(transpose(inverse(uModel))) * aNormal;
	fragWorldPosition = (uModel *vec4(aPos, 1.0)).xyz;
	outTexCoord = aTexCoord;
	gl_Position = uProjection *uView * uModel * vec4(aPos, 1.0);
}