#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

//struct MeshData {
//	std::vector<Vertex> vertices;
//	std::vector<unsigned int> indices;
//};

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

	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;

	GlMesh(const std::vector<Vertex> _vertices, const std::vector<unsigned int> _indices)
		: vertices(_vertices), indices(_indices) {



		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &EBO);

		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		//normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);

		//tex coord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

	};

	static std::unique_ptr<GlMesh> LoadMeshFromAssimp(const aiMesh* assimpMesh) {

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int i = 0; i < assimpMesh->mNumVertices; i++) {

			const aiVector3D vertPos = assimpMesh->mVertices[i];

			aiVector3D vertNormal{ 0.0f };
			if (assimpMesh->HasNormals()) {
				vertNormal = assimpMesh->mNormals[i];
			}
			aiVector3D vertTexCoord{ 0.0f };
			if (assimpMesh->HasTextureCoords(0)) {
				vertTexCoord = assimpMesh->mTextureCoords[0][i];
			}

			const glm::vec3 glmVertPos = glm::vec3{ vertPos.x, vertPos.y, vertPos.z };
			const glm::vec3 glmVertNormal = glm::vec3{ vertNormal.x, vertNormal.y, vertNormal.z };
			const glm::vec2 glmVertTexCoord = glm::vec2{ vertTexCoord.x, vertTexCoord.y };
			vertices.push_back(Vertex{ glmVertPos, glmVertNormal, glmVertTexCoord });


		}


		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < assimpMesh->mNumFaces; i++)
		{
			aiFace face = assimpMesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		return std::make_unique<GlMesh>(vertices, indices);

	}

	void Draw(const GlShaderProgram& Shader) const {


		Shader.Bind();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT,0 );
		glBindVertexArray(0);
	};

	//GlMesh(const GlMesh& rhs) = delete;
	//GlMesh& operator=(const GlMesh& rhs) = delete;

	//void Release() {
	//	glDeleteVertexArrays(1, &VAO);
	//	glDeleteBuffers(1, &VBO);
	//	glDeleteBuffers(1, &EBO);
	//}

	//GlMesh(GlMesh&& rhs) {
	//	
	//	VBO = rhs.VBO;
	//	VAO = rhs.VAO;
	//	EBO = rhs.EBO;
	//	vertices = rhs.vertices;
	//	indices = rhs.indices;

	//	rhs.VAO = 0;
	//	rhs.VBO = 0;
	//	rhs.EBO = 0;

	//};
	//GlMesh& operator=(GlMesh&& rhs) {

	//	if (&rhs != this) {
	//		std::swap(VAO, rhs.VAO);
	//		std::swap(VBO, rhs.VBO);
	//		std::swap(EBO, rhs.EBO);
	//		Release();
	//	}
	//};

};