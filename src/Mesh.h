#pragma once

#include "glad/glad.h"

#include <vector>
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>
#include <numeric>



class GlMesh {

public:
	size_t indices_size{};

	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;


	template<typename VertexType>
	GlMesh(const std::vector<VertexType>& vertices, const std::vector<int>& numFloatsPerAttr, const std::vector<unsigned int>& indices)
	{

		indices_size = indices.size();

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &EBO);

		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexType) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);


		setVertexAttributes(VAO, VBO, numFloatsPerAttr);
		glBindVertexArray(0);

	};

	
	void Draw(const GlShaderProgram& Shader) const {


		Shader.Bind();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT,0 );
		glBindVertexArray(0);
		Shader.Unbind();
	};

	GlMesh(const GlMesh& rhs) = delete;
	GlMesh& operator=(const GlMesh& rhs) = delete;

	~GlMesh() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

private:
	// WARNING: vertex data must be float only
	void setVertexAttributes(unsigned int VAO, unsigned int VBO, std::vector<int> numDataPerAttr) {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		int stride = std::accumulate(numDataPerAttr.begin(), numDataPerAttr.end(), 0) * sizeof(float);
		int offset = 0;
		for (int i = 0; i < numDataPerAttr.size(); i++)
		{
			glVertexAttribPointer(i, numDataPerAttr[i], GL_FLOAT, GL_FALSE, stride, (void*)(offset));
			glEnableVertexAttribArray(i);
			offset += numDataPerAttr[i] * sizeof(float);
		}
		glBindVertexArray(0);
	}

};