#pragma once


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "Mesh.h"

class GlModel {

public:

	GlMesh* MeshInstance = nullptr;
	

	GlModel(const std::string& ModelFilePath) {

		Assimp::Importer Importer{};
		const aiScene* scene = Importer.ReadFile(ModelFilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
		const aiMesh* mesh = scene->mMeshes[0];
		for (size_t i = 0; i < mesh->mNumVertices; i++) {

			const aiVector3D vertPos = mesh->mVertices[i];
			const aiVector3D vertNormal = mesh->mNormals[i];
			const aiVector3D vertTexCoord = mesh->mTextureCoords[0][i];
			
			const glm::vec3 glmVertPos = glm::vec3{ vertPos.x, vertPos.y, vertPos.z };
			const glm::vec3 glmVertNormal = glm::vec3{ vertNormal.x, vertNormal.y, vertNormal.z };
			const glm::vec2 glmVertTexCoord = glm::vec2{ vertTexCoord.x, vertTexCoord.y };

			std::vector<Vertex> vertices;
			vertices.push_back(Vertex{ glmVertPos, glmVertNormal, glmVertTexCoord });
			std::vector<unsigned int> indices;

			for (size_t i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];

				for (size_t i = 0; i < face.mNumIndices; i++)
				{

				}
			}
		/*	std::cout << mesh->mFaces[0].mIndices[0] ;*/
			
		}
		
		
	}
};