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

	void SetUniforms(Camera& Cam) {
		Material->Shader->Bind();
		Material->Shader->SetMatrix4f("uView", Cam.view);
		Material->Shader->SetMatrix4f("uProjection", Cam.projection);
		Material->Shader->SetVec3("Light.direction", DirLightDirection);
		Material->Shader->SetVec3("Light.color", LightColor);
		Material->Shader->SetVec3("PointLight.color", PointLightColor);
		Material->Shader->SetVec3("PointLight.position", PointLightPosition);
		Material->Shader->SetFloat("PointLight.constant", 0.80f);
		Material->Shader->SetFloat("PointLight.linear", 0.01f);
		Material->Shader->SetFloat("PointLight.quadratic", 0.032f);
		Material->Shader->SetVec3("uCameraPos", Cam.Position);
		Material->Shader->SetFloat("Mat.shine", 120.0);
		Material->Shader->SetFloat("Mat.emissionStrength", 15.0);
		Material->Shader->SetTexture("Mat.color", *MP7Diffuse, 0);
		Material->Shader->SetTexture("Mat.specular", *MP7Specular, 1);
		Material->Shader->SetTexture("Mat.emission", *MP7Emission, 2);
		Material->Shader->SetCubemapTexture("skybox", *SkyboxTex, 4);
		glm::mat4 gunModelMatrix{ 1.0 };
		Material->Shader->SetMatrix4f("uModel", gunModelMatrix);
	}
	void Draw(Camera& Cam) const {
		
		
		Mesh->Draw(*Material->Shader);
	}


};