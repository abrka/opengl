#version 330 core
layout (location = 0) in vec3 aPos;
									
out vec3 texCoords;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	texCoords = aPos;										
	gl_Position = uProjection *uView * vec4(aPos, 1.0);
}