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
#include "Model.h"
#include <memory>
#include "StbiImageRAII.h"
#include "CubemapTexture.h"

namespace AssetLoader {

	const unsigned int NumOfFacesInCubemap = 6;
	/*std::vector<std::shared_ptr<GlMesh>> LoadedMeshes;
	std::vector<std::shared_ptr<GlShaderProgram>> LoadedShaders;*/

	static std::filesystem::path LoadTexturePathFromAssimp(aiMaterial& mat, aiTextureType type) {
		aiString path{};
		mat.GetTexture(type, 0, &path);
		return std::filesystem::path{ path.C_Str() };
	}

	GLenum GetTextureFormat(unsigned int nrOfChannels) {

		switch (nrOfChannels)
		{
		case 3:
			return GL_RGB;
			break;
		case 4:
			return GL_RGBA;
			break;
		default:
			assert(false && "number of image channels not supported");
			break;
		}
	};

	GLenum GetInternalTextureFormat(unsigned int nrOfChannels) {

		switch (nrOfChannels)
		{
		case 3:
			return GL_SRGB;
			break;
		case 4:
			return GL_SRGB_ALPHA;
			break;
		default:
			assert(false && "number of image channels not supported");
			break;
		}
	}

	std::shared_ptr<GlTexture> LoadTextureFromPath(const std::filesystem::path path) {
		stbiImage stbTexture{ path };
		return std::make_shared<GlTexture>(GetInternalTextureFormat(stbTexture.nrOfChannels), GetTextureFormat(stbTexture.nrOfChannels), stbTexture.width, stbTexture.height, stbTexture.ImageData);
	}

	std::shared_ptr<GlCubemapTexture> LoadCubemapTextureFromPath(std::array<std::filesystem::path, NumOfFacesInCubemap>&& paths) {

		stbiImage	right{ paths[0] };
		stbiImage	left{ paths[1] };
		stbiImage	top{ paths[2] };
		stbiImage	bottom{ paths[3] };
		stbiImage	front{ paths[4] };
		stbiImage	back{ paths[5] };

		return std::make_shared<GlCubemapTexture>
			(
				GetInternalTextureFormat(left.nrOfChannels),
				GetTextureFormat(left.nrOfChannels),
				std::array<unsigned char*, 6>{right.ImageData, left.ImageData, top.ImageData, bottom.ImageData, front.ImageData, back.ImageData },
				std::array<int, 6>{ right.width, left.width, top.width, bottom.width, front.width, back.width },
				std::array<int, 6>{ right.height, left.height, top.height, bottom.height, front.height, back.height }
			);

	};

	static std::shared_ptr<GlTexture> LoadTextureFromAssimp(aiMaterial& mat, aiTextureType texType, std::filesystem::path TextureFolder) {

		std::filesystem::path texRelPath = LoadTexturePathFromAssimp(mat, texType);
		if (texRelPath.empty()) {
			return nullptr;
		}

		std::filesystem::path texPath = TextureFolder / texRelPath;

		return LoadTextureFromPath(texPath);
	};



	static std::shared_ptr<GlShaderProgram> LoadMaterialFromAssimp(aiMaterial& mat, std::filesystem::path Folder, std::filesystem::path DefaultVertexShader, std::filesystem::path DefaultFragShader) {


		std::shared_ptr<GlShaderProgram> LoadedShader = std::make_shared<GlShaderProgram>(DefaultFragShader.string(), DefaultVertexShader.string());
		LoadedShader->Bind();


		
		auto DiffuseTex = LoadTextureFromAssimp(mat, aiTextureType_BASE_COLOR, Folder);
		if (DiffuseTex) {
			LoadedShader->SetTexture("Mat.color", *DiffuseTex, 0);
		}
		else {
			std::cout << "Diffuse texture for material not found" << std::endl;
		}

		auto SpecularTex = LoadTextureFromAssimp(mat, aiTextureType_SPECULAR, Folder);
		if (SpecularTex) {
			LoadedShader->SetTexture("Mat.specular", *SpecularTex, 1);
		}
		else {
			std::cout << "specular texture for material not found" << std::endl;
		}

		auto EmissionTex = LoadTextureFromAssimp(mat, aiTextureType_EMISSIVE, Folder);
		if (EmissionTex) {
			LoadedShader->SetTexture("Mat.emission", *EmissionTex, 2);		
		}
		else {
			std::cout << "emissive texture for material not found" << std::endl;
		}


		LoadedShader->Unbind();

		return LoadedShader;

	}

	static std::shared_ptr<GlMesh> LoadMeshFromAssimp(aiMesh& mesh)
	{


		// data to fill
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;


		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			vector.x = mesh.mVertices[i].x;
			vector.y = mesh.mVertices[i].y;
			vector.z = mesh.mVertices[i].z;
			vertex.position = vector;
			// normals
			if (mesh.HasNormals())
			{
				vector.x = mesh.mNormals[i].x;
				vector.y = mesh.mNormals[i].y;
				vector.z = mesh.mNormals[i].z;
				vertex.normal = vector;
			}
			// texture coordinates
			if (mesh.mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh.mTextureCoords[0][i].x;
				vec.y = mesh.mTextureCoords[0][i].y;
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
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			aiFace face = mesh.mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}


		return std::make_shared<GlMesh>(vertices, indices);
	}




	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	/*static std::shared_ptr<GlModel> LoadModelFromAssimp(std::string const& path, const std::filesystem::path TextureFolder, const std::filesystem::path DefaultVertexShader, const std::filesystem::path DefaultFragShader)
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return nullptr;
		}


		LoadedMeshes.push_back(LoadMeshFromAssimp(scene->mMeshes[0]));
		LoadedShaders.push_back(LoadMaterialFromAssimp(*scene->mMaterials[0], TextureFolder, DefaultVertexShader, DefaultFragShader));

		return std::make_shared<GlModel>(*LoadedMeshes.back(), *LoadedShaders.back());

		// process ASSIMP's root node recursively
		//processNode(scene->mRootNode, scene);
	}*/

	//not used
	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	//static void processNode(aiNode* node, const aiScene* scene)
	//{
	//	std::vector<GlMesh> loadedMeshes;
	//	// process each mesh located at the current node
	//	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	//	{
	//		// the node object only contains indices to index the actual objects in the scene. 
	//		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
	//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	//		loadedMeshes.push_back(*GL::LoadMeshFromAssimp(mesh));
	//	}
	//	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	//	for (unsigned int i = 0; i < node->mNumChildren; i++)
	//	{
	//		processNode(node->mChildren[i], scene);
	//	}

	//}

}