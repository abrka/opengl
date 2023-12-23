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

	/*GlModel(std::shared_ptr<GlMesh> _Mesh, std::shared_ptr<GlShaderProgram> _Shader) :
		Mesh(_Mesh), Shader(_Shader){};*/


	GlModel(std::shared_ptr<GlMesh> _Mesh, std::shared_ptr<GlMaterial> _Material) :
		Mesh(_Mesh), Material(_Material) {};

	void SetUniforms(Camera& Cam, DirLight& DirLightSource, PointLight& PointLightSource, GlCubemapTexture& Skybox) const {
		Material->Shader->Bind();
		Material->Shader->SetMatrix4f("uView", Cam.view);
		Material->Shader->SetMatrix4f("uProjection", Cam.projection);
		Material->Shader->SetVec3("Light.direction", DirLightSource.Direction);
		Material->Shader->SetVec3("Light.color", DirLightSource.Color);
		Material->Shader->SetVec3("PointLight.color", PointLightSource.Color);
		Material->Shader->SetVec3("PointLight.position", PointLightSource.Position);
		Material->Shader->SetFloat("PointLight.constant", PointLightSource.Constant);
		Material->Shader->SetFloat("PointLight.linear", PointLightSource.Linear);
		Material->Shader->SetFloat("PointLight.quadratic", PointLightSource.Quadratic);
		Material->Shader->SetVec3("uCameraPos", Cam.Position);
		Material->Shader->SetFloat("Mat.shine", Material->Shine);
		Material->Shader->SetFloat("Mat.emissionStrength", Material->EmissionStrength);
		Material->Shader->SetTexture("Mat.color", *Material->DiffuseTex, 0);
		Material->Shader->SetTexture("Mat.specular", *Material->SpecularTex, 1);
		Material->Shader->SetTexture("Mat.emission", *Material->EmissionTex, 2);
		Material->Shader->SetCubemapTexture("skybox", Skybox, 4);
		Material->Shader->SetMatrix4f("uModel", ModelMatrix);
	};

	void Draw(Camera& Cam, DirLight& DirLightSource, PointLight& PointLightSource, GlCubemapTexture& Skybox) const {
		
		SetUniforms(Cam, DirLightSource, PointLightSource, Skybox);
		Mesh->Draw(*Material->Shader);
	}


};