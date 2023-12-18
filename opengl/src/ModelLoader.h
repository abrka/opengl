#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Texture.h"
#include "Shader.h"

namespace GL {

	
	std::vector<aiTextureType> AllTextureTypes{ aiTextureType_BASE_COLOR, aiTextureType_SPECULAR, aiTextureType_METALNESS, aiTextureType_EMISSIVE };
	std::vector<std::shared_ptr<GlTexture>> LoadedTextures;
	std::vector<std::shared_ptr<GlShaderProgram>> LoadedShaders;

	static std::filesystem::path LoadTexturePathFromAssimp(aiMaterial& mat, aiTextureType type) {
		aiString path{};
		mat.GetTexture(type, 0, &path);
		return std::filesystem::path{ path.C_Str() };
	}

	static std::shared_ptr<GlTexture> LoadTextureFromAssimp(aiMaterial& mat, aiTextureType texType, std::filesystem::path TextureFolder) {

		std::filesystem::path texRelPath = LoadTexturePathFromAssimp(mat, texType);
		if (texRelPath.empty()) {
			return nullptr;
		}

		std::filesystem::path texPath = TextureFolder / texRelPath;

		return std::make_shared<GlTexture>(texPath);
	}

	static void LoadAllTexturesFromAssimpMaterial(aiMaterial& mat, std::filesystem::path TextureFolder) {

		for (auto& TexType : AllTextureTypes)
		{
			LoadedTextures.push_back(LoadTextureFromAssimp(mat, TexType, TextureFolder));
		}
	}

	static std::shared_ptr<GlShaderProgram> LoadMaterialFromAssimp(aiMaterial& mat, std::filesystem::path Folder, std::filesystem::path DefaultVertexShader, std::filesystem::path DefaultFragShader ) {

		
		std::shared_ptr<GlShaderProgram> LoadedShader = std::make_shared<GlShaderProgram>(DefaultFragShader.string(), DefaultVertexShader.string());
		LoadedShader->Bind();

		auto DiffuseTex = LoadTextureFromAssimp(mat, aiTextureType_BASE_COLOR, Folder);
		if (DiffuseTex) {
			LoadedShader->SetTexture("mat.diffuse", *DiffuseTex, 0);
		}

		auto SpecularTex = LoadTextureFromAssimp(mat, aiTextureType_SPECULAR, Folder);
		if (SpecularTex) {
			LoadedShader->SetTexture("mat.specular", *SpecularTex, 1);
		}

		auto EmissionTex = LoadTextureFromAssimp(mat, aiTextureType_EMISSIVE, Folder);
		if (EmissionTex) {
			LoadedShader->SetTexture("mat.emission", *EmissionTex, 2);
		}

		return LoadedShader;
			
	}

	static std::shared_ptr<GlMesh> LoadMeshFromAssimp(aiMesh* mesh)
	{


		// data to fill
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;


		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.position = vector;
			// normals
			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.normal = vector;
			}
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoord = vec;
				//// tangent
				//vector.x = mesh->mTangents[i].x;
				//vector.y = mesh->mTangents[i].y;
				//vector.z = mesh->mTangents[i].z;
				//vertex.Tangent = vector;
				//// bitangent
				//vector.x = mesh->mBitangents[i].x;
				//vector.y = mesh->mBitangents[i].y;
				//vector.z = mesh->mBitangents[i].z;
				//vertex.Bitangent = vector;
			}
			else
				vertex.texCoord = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}


		return std::make_shared<GlMesh>(vertices, indices);
	}

}