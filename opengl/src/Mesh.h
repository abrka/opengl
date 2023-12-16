#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shader.h"


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

std::ostream& operator<<(std::ostream& os, const glm::vec3& v) {
	os << "x: " << v.x << " y: " << v.y << " z: " << v.z;
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec2& v) {
	os << "x: " << v.x << " y: " << v.y;
	return os;
}

std::ostream& operator<<(std::ostream& os, const Vertex& vt)
{
	os << "position " << vt.position << " normal " << vt.normal << " tex coord " << vt.texCoord;
	return os;
}

class GlMesh {

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	GlShaderProgram& Shader;


	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	GlMesh(std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices, GlShaderProgram& _Shader)
		: vertices(_vertices), indices(_indices), Shader(_Shader) {

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &EBO);

		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* indices.size(), &indices[0], GL_STATIC_DRAW);

		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		//normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);

		//tex coord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
		glEnableVertexAttribArray(2);
	};

	void Draw() const{
		Shader.Bind();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	};
};