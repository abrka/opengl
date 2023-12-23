#pragma once


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "Mesh.h"
#include <memory>
#include "Camera.h"
#include "Material.h"


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

	void SetUniforms(Camera& Cam, DirLight& DirLightSource, PointLight& PointLightSource, GlCubemapTexture& Skybox) const {
		Material->Shader->Bind();
		Material->Shader->SetMatrix4f(Material->ModelMatrixUniform, ModelMatrix);
		Material->Shader->SetMatrix4f(Material->ViewMatrixUniform, Cam.view);
		Material->Shader->SetMatrix4f(Material->ProjectionMatrixUniform, Cam.projection);
		Material->Shader->SetVec3(Material->DirLightDirectionUniform, DirLightSource.Direction);
		Material->Shader->SetVec3(Material->DirLightColorUniform, DirLightSource.Color);
		Material->Shader->SetVec3(Material->PointLightColorUniform, PointLightSource.Color);
		Material->Shader->SetVec3(Material->PointLightPositionUniform, PointLightSource.Position);
		Material->Shader->SetFloat(Material->PointLightConstantUniform, PointLightSource.Constant);
		Material->Shader->SetFloat(Material->PointLightLinearUniform, PointLightSource.Linear);
		Material->Shader->SetFloat(Material->PointLightQuadraticUniform, PointLightSource.Quadratic);
		Material->Shader->SetVec3(Material->CameraPositionUniform, Cam.Position);
		Material->Shader->SetFloat(Material->ShineUniform, Material->Shine);
		Material->Shader->SetFloat(Material->EmissionStrengthUniform, Material->EmissionStrength);
		Material->Shader->SetFloat(Material->ReflectionStrengthUniform, Material->ReflectionStrength);

		if (Material->DiffuseTex)
		{
			Material->Shader->SetTexture(Material->DiffuseTexUniform, *Material->DiffuseTex, 0);
		}
		if (Material->SpecularTex)
		{
			Material->Shader->SetTexture(Material->SpecularTexUniform, *Material->SpecularTex, 1);
		}
		if (Material->EmissionTex)
		{
			Material->Shader->SetTexture(Material->EmissionTexUniform, *Material->EmissionTex, 2);
		}

		Material->Shader->SetCubemapTexture(Material->SkyboxUniform, Skybox, 4);

		Material->Shader->Unbind();

	};

	void Draw(Camera& Cam, DirLight& DirLightSource, PointLight& PointLightSource, GlCubemapTexture& Skybox) {

		ModelMatrix = glm::mat4{ 1.0f };
		ModelMatrix = glm::scale(ModelMatrix, Scale);
		ModelMatrix = glm::rotate(ModelMatrix, EulerRotation.x, XAxis);
		ModelMatrix = glm::rotate(ModelMatrix, EulerRotation.y, YAxis);
		ModelMatrix = glm::rotate(ModelMatrix, EulerRotation.z, ZAxis);
		ModelMatrix = glm::translate(ModelMatrix, Translation);

		SetUniforms(Cam, DirLightSource, PointLightSource, Skybox);
		Mesh->Draw(*Material->Shader);
	}


};