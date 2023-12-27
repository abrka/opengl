#pragma once


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "Mesh.h"
#include <memory>
#include "Camera.h"
#include "Material.h"
#include "RendererContext.h"


class GlModel {

public:

	std::shared_ptr<GlMesh> Mesh;
	std::shared_ptr<GlMaterial> Material;
	glm::mat4 ModelMatrix{ 1.0f };
	glm::vec3 Translation{ 0.0f };
	glm::vec3 Scale{ 1.0f };
	glm::vec3 EulerRotation{ 0.0f };

	const glm::vec3 XAxis{ 1.0f,0.0f,0.0f };
	const glm::vec3 YAxis{ 0.0f,1.0f,0.0f };
	const glm::vec3 ZAxis{ 0.0f,0.0f,1.0f };



	GlModel(std::shared_ptr<GlMesh> _Mesh, std::shared_ptr<GlMaterial> _Material) :
		Mesh(_Mesh), Material(_Material) {};

	

	void Draw(GlRendererContext& RenderContext) {


		ModelMatrix = glm::mat4{ 1.0f };
		ModelMatrix = glm::scale(ModelMatrix, Scale);
		ModelMatrix = glm::rotate(ModelMatrix, EulerRotation.x, XAxis);
		ModelMatrix = glm::rotate(ModelMatrix, EulerRotation.y, YAxis);
		ModelMatrix = glm::rotate(ModelMatrix, EulerRotation.z, ZAxis);
		ModelMatrix = glm::translate(ModelMatrix, Translation);

		Material->SetUniforms(RenderContext, ModelMatrix);

		Mesh->Draw(*Material->Shader);
	}


};